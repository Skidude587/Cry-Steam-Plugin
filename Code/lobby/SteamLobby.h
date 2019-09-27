//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.h
//  Created:     23/09/2019 by Von Bismarck, Jonathan Green
//  Description: This Class file is for calling Steam API for our lobby system
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////


#pragma once 

#include "StdAfx.h"
#include <steam_api.h>
#include <steam_gameserver.h>
#include <PlatformTypes.h>
#include <PlatformIdentifier.h>
#include <IPlatformService.h>

//invites are session based
#define CryInviteID      CrySessionID
#define CryInvalidInvite CrySessionInvalidID
//structs



class CSteamLobby
{

	struct IUser* m_pUser;

public:
	CSteamLobby() = default;
	virtual ~CSteamLobby() {}



	////////////////////////////////////////////////////////
	// Basic Lobby for Cryengine works for Steam / Online ( dedicated or hosted ) , LAN, PSN, and Xbox won't make more for it right now..
	///////////////////////////////////////////////////////

	/* simple init if need */
	void Init();

	/* Might need to move this */
	void GetLocalClient();

	/* Sets lobby */
	void SetLobby();

	/* Refresh lobby */
	void RefreshLobby();

	/* Return services */

protected:

	Cry::GamePlatform::IPlugin *m_pPlugin;
	Cry::GamePlatform::IService *m_pISerivce;
	Cry::GamePlatform::AccountIdentifier *m_pAccountID;
};

