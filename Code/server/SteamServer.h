//
// -------------------------------------------------------------------------
//  File name:   SteamServer.h
//  Created:     23/09/2019 by Joey Gonzalez, Jonathan Green
//  Description: Setup for P2P sessions and IServer
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#pragma once 
#include "StdAfx.h"
#include <steam_api.h>
#include <steamclientpublic.h>
#include <steam_gameserver.h>

#include <PlatformTypes.h>
#include <PlatformIdentifier.h>
#include <IPlatformService.h>


class CSteamServer
{

	struct IUser* m_pUser;

public:
	CSteamServer() = default;
	virtual ~CSteamServer() {}



	////////////////////////////////////////////////////////
	// Basic Server for Cryengine works for Steam / Online ( dedicated or hosted ) , LAN, PSN, and Xbox won't make more for it right now..
	///////////////////////////////////////////////////////

	/* simple init if need */
	void Init();

	/* Get Server Info */
	void GetServerInfo();

	/* Sets Server */
	void SetServer();

	/* Refresh lobby */
	void tick();

	/* Return services */

protected:

	/* Add more pointer below when need creating server functions to call upon remember Cry::GamePlatform::    Allows to select all avaliable platform base functions */
	Cry::GamePlatform::IPlugin *m_pPlugin;
	Cry::GamePlatform::IService *m_pISerivce;
	Cry::GamePlatform::AccountIdentifier *m_pAccountID;
};
