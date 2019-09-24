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
#include "steam_int_h.h"
#include "public/steam/steam_api.h"

//Calls up Steam API lib
#if USING_STEAM
#pragma comment(lib, "steam_api64.lib")
S_API bool S_CALLTYPE SteamAPI_Init();
//

#include "CryLobby.h"
//int LobbyCVars for steam lobbies 
	int useSteamAsOnlineLobby;
	int resetSteamAchievementsOnBoot;
	int lobbySteamOnlinePort;
#endif
//
