/*
 *  game-genie.cpp
 *  Genie
 *
 *  Created by Lucas on 04.04.10.
 *  Copyright 2010 Lucas Romero. All rights reserved.
 *
 */

#include "ghost-genie.h"
#include "game-genie.h"
#include "game.h"
#include "game_base.h"

CGameGenie :: CGameGenie( CGHostGenie *nGHost, CMap *nMap, CSaveGame *nSaveGame, uint16_t nHostPort, unsigned char nGameState, string nGameName, string nOwnerName, string nCreatorName, string nCreatorServer )
	//: CGame( nGHost, nMap, nSaveGame, nHostPort, nGameState, nGameName, nOwnerName, nCreatorName, nCreatorServer ), m_GHostGenie( nGHost )
	: CGame ( nGHost, nMap, nSaveGame, nHostPort, nGameState, nGameName, nOwnerName, nCreatorName, nCreatorServer ), m_GHostGenie( nGHost )
{
	
}

void CGameGenie :: EventGameLoaded( )
{
	CBaseGame :: EventGameLoaded( );
	m_GHostGenie->EventGameLoaded( this );
}