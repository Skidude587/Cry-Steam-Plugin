//
//
//
#include "StdAfx.h"
#include "SteamServer.h"
#include "lobby/steam_int_h.h"
#include "public/steam/steam_api.h"

void cSteamServer::OnP2PSessionRequest(P2PSessionRequest_t* pCallback)
{
	// If returned false, steamIDRemote is invalid. 
	if (!SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		// Add error log ...

	}
}