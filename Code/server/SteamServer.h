//
// -------------------------------------------------------------------------
//  File name:   SteamServer.h
//  Created:     23/09/2019 by Joey Gonzalez, 
//  Description: Class header for P2P sessions
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#pragma once 
#include "StdAfx.h"


class cSteamServer
{
public:
	void CloseP2PSession(CSteamID steamIDRemote);
	bool IsP2PPacketAvailable(uint32* pMsgSize);
	bool ReadP2PPacket(void* pDest, uint32 destSize, uint32* pMsgSize, CSteamID* pSteamIDRemote);
private:
	STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t);
	STEAM_CALLBACK(cSteamServer, OnP2PSessionConnectFail, P2PSessionConnectFail_t);

};