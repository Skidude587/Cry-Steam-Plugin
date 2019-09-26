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
#include "friends/SteamFriends.h"
#include "server/SteamServer.h"
#include <steam_api.h>
#include "server/NetLog.h"
#include <CryLobby/CommonICryLobby.h>
#include <IGamePlatform.h>
#include <PlatformTypes.h>

void CSteamLobby::Init()
{
	GetLocalClient();

	if (!m_pPlugin->GetService(m_pServices->Service))
	{
		CryLog("Something went wrong -- on Service");
	}

	//TODO Finish add server and lobby 

}

void CSteamLobby::GetLocalClient()
{
	if (!gEnv->IsEditor())
		return;
	//TODO make this do something besides success lol
	if (m_pPlugin->GetLocalClient())
	{
		CryLog("============================  Success! %d   ==============================", m_pPlugin->GetLocalClient());
		
	}
	else
	{
		CryLog("Unable to find user!");
	}

}

