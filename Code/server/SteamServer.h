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

private:
	STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t);
};