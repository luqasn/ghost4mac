/*
 *  ghost-genie.cpp
 *  Genie
 *
 *  Created by Lucas on 28.03.10.
 *  Copyright 2010 Lucas Romero. All rights reserved.
 *
 */

#include <string>

#include "ghost-genie.h"
#include "bnet-genie.h"

#include "csvparser.h"
#include "util.h"
#include "ghostdb.h"
#include "game_base.h"
#include "game_admin.h"
#include "game-genie.h"
#include "config.h"
#include "language.h"
#include "packed.h"
#include "savegame.h"
#include "map.h"
#include "gameprotocol.h"
#include "sqlite3.h"

CGHostGenie :: CGHostGenie( MessageLogger *logger, CConfig *CFG ) : CGHost( logger, CFG ),
hashCallback( NULL ),
bnetCallback( NULL ),
gameCallback( NULL ),
ip2countryCallback( NULL)
{
	bool enableRemoteHash = CFG->GetInt( "genie_remotehash", 1 ) == 0 ? false : true;
	vector<CBNET*> newBNETs;
	for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
	{
		CBNETGenie *bnet = new CBNETGenie( this, *i, enableRemoteHash );
		newBNETs.push_back( bnet );
	}
	// swap bnet lists
	m_BNETs.swap(newBNETs);
	// newBNETs now contains the old bnet instances that can be thrown away
	newBNETs.clear();
}

void CGHostGenie :: LoadIPToCountryData( )
{
	// just don't load it yet, we will call LoadIPToCountryData(string) later
	// blocking the call
	// CGHost :: LoadIPToCountryData( );
}

void CGHostGenie :: LoadIPToCountryData( string file )
{
	ifstream in;
	in.open( file.c_str( ) );
	
	if( in.fail( ) )
		LogWarning( "[GHOST] warning - unable to read file [" + file + "], iptocountry data not loaded" );
	else
	{
		LogInfo( "[GHOST] started loading [" + file + "]" );
		
		// the begin and commit statements are optimizations
		// we're about to insert ~4 MB of data into the database so if we allow the database to treat each insert as a transaction it will take a LONG time
		// todotodo: handle begin/commit failures a bit more gracefully
		
		if( !m_DBLocal->Begin( ) )
			LogWarning( "[GHOST] warning - failed to begin local database transaction, iptocountry data not loaded" );
		else
		{
			unsigned char Percent = 0;
			string Line;
			string IP1;
			string IP2;
			string Country;
			CSVParser parser;
			
			// get length of file for the progress meter
			
			in.seekg( 0, ios :: end );
			uint32_t FileLength = in.tellg( );
			in.seekg( 0, ios :: beg );
			
			while( !in.eof( ) )
			{
				getline( in, Line );
				
				if( Line.empty( ) )
					continue;
				
				parser << Line;
				parser >> IP1;
				parser >> IP2;
				parser >> Country;
				m_DBLocal->FromAdd( UTIL_ToUInt32( IP1 ), UTIL_ToUInt32( IP2 ), Country );
				
				// it's probably going to take awhile to load the iptocountry data (~10 seconds on my 3.2 GHz P4 when using SQLite3)
				// so let's print a progress meter just to keep the user from getting worried
				
				unsigned char NewPercent = (unsigned char)( (float)in.tellg( ) / FileLength * 100 );
				
				if( NewPercent != Percent )
				{
					if( NewPercent % 10 == 0 )
						LogInfo( "[GHOST] iptocountry data: " + UTIL_ToString( NewPercent ) + "% loaded" );
					Percent = NewPercent;
					if( ip2countryCallback )
						ip2countryCallback( callbackObject, NewPercent );
				}
			}
			
			if( !m_DBLocal->Commit( ) )
				LogWarning( "[GHOST] warning - failed to commit local database transaction, iptocountry data not loaded" );
			else
				LogInfo( "[GHOST] finished loading [ip-to-country.csv]" );
		}
		
		in.close( );
	}
}

void CGHostGenie :: OnBNETMessage( BNETEventType type, CBNET *bnet, const string &user, const string &msg)
{
	BNETEventData d;
	d.bnet = bnet;
	d.event = type;
	d.user = user;
	d.message = msg;
	OnBNETEvent( d );
}

void CGHostGenie :: OnBNETEvent( BNETEventType type, CBNET *bnet )
{
	BNETEventData d;
	d.bnet = bnet;
	d.event = type;
	OnBNETEvent( d );
}

void CGHostGenie :: OnBNETEvent( BNETEventData &data)
{
	if( bnetCallback != NULL )
		bnetCallback( callbackObject, data );
}

void CGHostGenie :: EventBNETHashRequest( const EventBNETHashRequestData &data )
{
	if( hashCallback != NULL )
		hashCallback( callbackObject, data );
}

void CGHostGenie :: EventBNETIncomingFriend( CBNET *bnet, string user, string status )
{
	OnBNETMessage( BNETEventTypeIncomingFriend, bnet, user, status );
}

void CGHostGenie :: EventBNETIncomingClanMember( CBNET *bnet, string user, string status )
{
	OnBNETMessage( BNETEVentTypeIncomingClanMember, bnet, user, status );
}

void CGHostGenie :: EventBNETConnecting( CBNET *bnet )
{
	CGHost :: EventBNETConnecting( bnet );
	OnBNETEvent( BNETEventTypeConnecting, bnet );
}

void CGHostGenie :: EventBNETConnected( CBNET *bnet )
{
	CGHost :: EventBNETConnected( bnet );
	OnBNETEvent( BNETEventTypeConnected, bnet );
}

void CGHostGenie :: EventBNETChannelJoined( CBNET *bnet, string user, string channel )
{
	OnBNETMessage( BNETEventTypeChannelJoined, bnet, user, channel );
}

void CGHostGenie :: EventBNETChatLeft( CBNET *bnet )
{
	OnBNETEvent( BNETEventTypeChatLeft, bnet );
}

void CGHostGenie :: EventBNETUserJoinedChannel( CBNET *bnet, string user, string channel )
{
	OnBNETMessage( BNETEventTypeUserJoinedChannel, bnet, user, channel );
}

void CGHostGenie :: EventBNETUserLeftChannel( CBNET *bnet, string user, string channel )
{
	OnBNETMessage( BNETEventTypeUserLeftChannel, bnet, user, channel );
}

void CGHostGenie :: EventBNETDisconnected( CBNET *bnet )
{
	CGHost :: EventBNETDisconnected( bnet );
	OnBNETEvent( BNETEventTypeDisconnected, bnet );
}

void CGHostGenie :: EventBNETLoggedIn( CBNET *bnet )
{
	CGHost :: EventBNETLoggedIn( bnet );
	OnBNETEvent( BNETEventTypeLoggedIn, bnet );
}

void CGHostGenie :: EventBNETGameRefreshed( CBNET *bnet )
{
	CGHost :: EventBNETGameRefreshed( bnet );
	OnBNETEvent( BNETEventTypeGameRefreshed, bnet );
}

void CGHostGenie :: EventBNETGameRefreshFailed( CBNET *bnet )
{
	CGHost :: EventBNETGameRefreshFailed( bnet );
	OnBNETEvent( BNETEventTypeGameRefreshFailed, bnet );
}

void CGHostGenie :: EventBNETConnectTimedOut( CBNET *bnet )
{
	CGHost :: EventBNETConnectTimedOut( bnet );
	OnBNETEvent( BNETEventTypeConnectTimeout, bnet );
}

void CGHostGenie :: EventBNETWhisper( CBNET *bnet, string user, string message )
{
	CGHost :: EventBNETWhisper( bnet, user, message );
	OnBNETMessage( BNETEventTypeWhisper, bnet, user, message );
}

void CGHostGenie :: EventBNETChat( CBNET *bnet, string user, string message )
{
	CGHost :: EventBNETChat( bnet, user, message );
	OnBNETMessage( BNETEventTypeChat, bnet, user, message );
}

void CGHostGenie :: EventBNETEmote( CBNET *bnet, string user, string message )
{
	CGHost :: EventBNETEmote( bnet, user, message );
	OnBNETMessage( BNETEventTypeEmote, bnet, user, message );
}

void CGHostGenie :: OnGameEvent( GameEventType type, CBaseGame *game)
{
	GameEventData data;
	data.event = type;
	data.game = game;
	if( gameCallback != NULL )
		gameCallback( callbackObject, data );
}

void CGHostGenie :: EventGameRefreshed( CBaseGame* game, const string &gamename, unsigned char state )
{
	OnGameEvent( GameEventTypeRefreshed, game );
}

void CGHostGenie :: EventGameLoaded( CBaseGame* game )
{
	OnGameEvent( GameEventTypeLoaded, game );
}

/*void CGHostGenie :: CreateGame( CMap *map, unsigned char gameState, bool saveGame, string gameName, string ownerName, string creatorName, string creatorServer, bool whisper )
{
	CBaseGame *oldGame = m_CurrentGame;
	CGHost :: CreateGame( map, gameState, saveGame, gameName, ownerName, creatorName, creatorServer, whisper );
	if( m_CurrentGame && oldGame != m_CurrentGame )
	{
		// new game was hosted
		OnGameEvent( GameEventTypeCreated, m_CurrentGame );
	}
}*/

void CGHostGenie :: CreateGame( CMap *map, unsigned char gameState, bool saveGame, string gameName, string ownerName, string creatorName, string creatorServer, bool whisper )
{
	if( !m_Enabled )
	{
		for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
		{
			if( (*i)->GetServer( ) == creatorServer )
				(*i)->QueueChatCommand( m_Language->UnableToCreateGameDisabled( gameName ), creatorName, whisper );
		}
		
		if( m_AdminGame )
			m_AdminGame->SendAllChat( m_Language->UnableToCreateGameDisabled( gameName ) );
		
		return;
	}
	
	if( gameName.size( ) > 31 )
	{
		for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
		{
			if( (*i)->GetServer( ) == creatorServer )
				(*i)->QueueChatCommand( m_Language->UnableToCreateGameNameTooLong( gameName ), creatorName, whisper );
		}
		
		if( m_AdminGame )
			m_AdminGame->SendAllChat( m_Language->UnableToCreateGameNameTooLong( gameName ) );
		
		return;
	}
	
	if( !map->GetValid( ) )
	{
		for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
		{
			if( (*i)->GetServer( ) == creatorServer )
				(*i)->QueueChatCommand( m_Language->UnableToCreateGameInvalidMap( gameName ), creatorName, whisper );
		}
		
		if( m_AdminGame )
			m_AdminGame->SendAllChat( m_Language->UnableToCreateGameInvalidMap( gameName ) );
		
		return;
	}
	
	if( saveGame )
	{
		if( !m_SaveGame->GetValid( ) )
		{
			for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
			{
				if( (*i)->GetServer( ) == creatorServer )
					(*i)->QueueChatCommand( m_Language->UnableToCreateGameInvalidSaveGame( gameName ), creatorName, whisper );
			}
			
			if( m_AdminGame )
				m_AdminGame->SendAllChat( m_Language->UnableToCreateGameInvalidSaveGame( gameName ) );
			
			return;
		}
		
		string MapPath1 = m_SaveGame->GetMapPath( );
		string MapPath2 = map->GetMapPath( );
		transform( MapPath1.begin( ), MapPath1.end( ), MapPath1.begin( ), (int(*)(int))tolower );
		transform( MapPath2.begin( ), MapPath2.end( ), MapPath2.begin( ), (int(*)(int))tolower );
		
		if( MapPath1 != MapPath2 )
		{
			CONSOLE_Print( "[GHOST] path mismatch, saved game path is [" + MapPath1 + "] but map path is [" + MapPath2 + "]" );
			
			for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
			{
				if( (*i)->GetServer( ) == creatorServer )
					(*i)->QueueChatCommand( m_Language->UnableToCreateGameSaveGameMapMismatch( gameName ), creatorName, whisper );
			}
			
			if( m_AdminGame )
				m_AdminGame->SendAllChat( m_Language->UnableToCreateGameSaveGameMapMismatch( gameName ) );
			
			return;
		}
		
		if( m_EnforcePlayers.empty( ) )
		{
			for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
			{
				if( (*i)->GetServer( ) == creatorServer )
					(*i)->QueueChatCommand( m_Language->UnableToCreateGameMustEnforceFirst( gameName ), creatorName, whisper );
			}
			
			if( m_AdminGame )
				m_AdminGame->SendAllChat( m_Language->UnableToCreateGameMustEnforceFirst( gameName ) );
			
			return;
		}
	}
	
	if( m_CurrentGame )
	{
		for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
		{
			if( (*i)->GetServer( ) == creatorServer )
				(*i)->QueueChatCommand( m_Language->UnableToCreateGameAnotherGameInLobby( gameName, m_CurrentGame->GetDescription( ) ), creatorName, whisper );
		}
		
		if( m_AdminGame )
			m_AdminGame->SendAllChat( m_Language->UnableToCreateGameAnotherGameInLobby( gameName, m_CurrentGame->GetDescription( ) ) );
		
		return;
	}
	
	if( m_Games.size( ) >= m_MaxGames )
	{
		for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
		{
			if( (*i)->GetServer( ) == creatorServer )
				(*i)->QueueChatCommand( m_Language->UnableToCreateGameMaxGamesReached( gameName, UTIL_ToString( m_MaxGames ) ), creatorName, whisper );
		}
		
		if( m_AdminGame )
			m_AdminGame->SendAllChat( m_Language->UnableToCreateGameMaxGamesReached( gameName, UTIL_ToString( m_MaxGames ) ) );
		
		return;
	}
	
	CONSOLE_Print( "[GHOST] creating game [" + gameName + "]" );
	
	if( saveGame )
		m_CurrentGame = new CGameGenie( this, map, m_SaveGame, m_HostPort, gameState, gameName, ownerName, creatorName, creatorServer );
	else
		m_CurrentGame = new CGameGenie( this, map, NULL, m_HostPort, gameState, gameName, ownerName, creatorName, creatorServer );
	
	// todotodo: check if listening failed and report the error to the user
	
	if( m_SaveGame )
	{
		m_CurrentGame->SetEnforcePlayers( m_EnforcePlayers );
		m_EnforcePlayers.clear( );
	}
	
	for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
	{
		if( whisper && (*i)->GetServer( ) == creatorServer )
		{
			// note that we send this whisper only on the creator server
			
			if( gameState == GAME_PRIVATE )
				(*i)->QueueChatCommand( m_Language->CreatingPrivateGame( gameName, ownerName ), creatorName, whisper );
			else if( gameState == GAME_PUBLIC )
				(*i)->QueueChatCommand( m_Language->CreatingPublicGame( gameName, ownerName ), creatorName, whisper );
		}
		else
		{
			// note that we send this chat message on all other bnet servers
			
			if( gameState == GAME_PRIVATE )
				(*i)->QueueChatCommand( m_Language->CreatingPrivateGame( gameName, ownerName ) );
			else if( gameState == GAME_PUBLIC )
				(*i)->QueueChatCommand( m_Language->CreatingPublicGame( gameName, ownerName ) );
		}
		
		if( saveGame )
			(*i)->QueueGameCreate( gameState, gameName, string( ), map, m_SaveGame, m_CurrentGame->GetHostCounter( ) );
		else
			(*i)->QueueGameCreate( gameState, gameName, string( ), map, NULL, m_CurrentGame->GetHostCounter( ) );
	}
	
	if( m_AdminGame )
	{
		if( gameState == GAME_PRIVATE )
			m_AdminGame->SendAllChat( m_Language->CreatingPrivateGame( gameName, ownerName ) );
		else if( gameState == GAME_PUBLIC )
			m_AdminGame->SendAllChat( m_Language->CreatingPublicGame( gameName, ownerName ) );
	}
	
	// if we're creating a private game we don't need to send any game refresh messages so we can rejoin the chat immediately
	// unfortunately this doesn't work on PVPGN servers because they consider an enterchat message to be a gameuncreate message when in a game
	// so don't rejoin the chat if we're using PVPGN
	
	if( gameState == GAME_PRIVATE )
	{
		for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
		{
			if( (*i)->GetPasswordHashType( ) != "pvpgn" )
				(*i)->QueueEnterChat( );
		}
	}
	
	// hold friends and/or clan members
	
	for( vector<CBNET *> :: iterator i = m_BNETs.begin( ); i != m_BNETs.end( ); i++ )
	{
		if( (*i)->GetHoldFriends( ) )
			(*i)->HoldFriends( m_CurrentGame );
		
		if( (*i)->GetHoldClan( ) )
			(*i)->HoldClan( m_CurrentGame );
	}
	
	OnGameEvent( GameEventTypeCreated, m_CurrentGame );
}

void CGHostGenie :: EventGameDeleted( CBaseGame *game )
{
	CGHost :: EventGameDeleted( game );
	OnGameEvent( GameEventTypeDeleted, game );
}