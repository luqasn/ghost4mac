/*
 *  game-genie.h
 *  Genie
 *
 *  Created by Lucas on 04.04.10.
 *  Copyright 2010 Lucas Romero. All rights reserved.
 *
 */

#ifndef CGAMEGENIE_H
#define CGAMEGENIE_H

#include <string>
using namespace std;
//#include "game_base.h"
#include "includes.h"
#include "ghost.h"
#include "game_base.h"
#include "game.h"

class CGHostGenie;
class CSaveGame;
class CMap;
class CPotentialPlayer;
class CIncomingJoinPlayer;
class CGamePlayer;



class CGameGenie : public CGame
{
protected:
	CGHostGenie *m_GHostGenie;
public:
	CGameGenie( CGHostGenie *nGHost, CMap *nMap, CSaveGame *nSaveGame, unsigned short nHostPort, unsigned char nGameState, string nGameName, string nOwnerName, string nCreatorName, string nCreatorServer );
	//virtual void EventPlayerJoined( CPotentialPlayer *potential, CIncomingJoinPlayer *joinPlayer );
	//virtual void EventPlayerChangeTeam( CGamePlayer *player, unsigned char team );
	//virtual void EventPlayerChangeColour( CGamePlayer *player, unsigned char colour );
	//virtual void EventPlayerChatToHost( CGamePlayer *player, CIncomingChatPlayer *chatPlayer );
	//virtual void EventPlayerLeft( CGamePlayer *player, uint32_t reason );
	//virtual void EventGameStarted( );
	virtual void EventGameLoaded( );
};

#endif