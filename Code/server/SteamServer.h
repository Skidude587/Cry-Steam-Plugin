//
//	File: SteamServer.h
//	
//
//

#pragma once 
#include "StdAfx.h"
#include "lobby/steam_int_h.h"
#include "public/steam/steam_api.h"

class cSteamServer
{
public:
	void CloseP2PSession(CSteamID steamIDRemote);

private:
	STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t);
	STEAM_CALLBACK(cSteamServer, OnP2PSessionConnectFail, P2PSessionConnectFail_t);

};