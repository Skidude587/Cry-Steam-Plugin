//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.cpp
//  Created:     23/09/2019 by Jonathan Green, 
//  Description: Creating the Multiplayer Steam Lobby
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SteamLobby.h"

//Calls up Steam API lin
#if USING_STEAM
#pragma comment(lib, "steam_api64.lib")

#include "CryLobby.h"

//int LobbyCVars for steam lobbies 
	int useSteamAsOnlineLobby;
	int resetSteamAchievementsOnBoot;
	int lobbySteamOnlinePort;
#endif
