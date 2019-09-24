//
//	File: SteamServer.cpp
//	
//
//

#include "StdAfx.h"
#include "SteamServer.h"
#include "lobby/steam_int_h.h"
#include "public/steam/steam_api.h"


// Callback for recieving P2P requests.
void cSteamServer::OnP2PSessionRequest(P2PSessionRequest_t* pCallback)
{
	// If returned false, steamIDRemote is invalid. 
	if (!SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		// Add error log ...

	}
}

// Callback for when a P2P connection fails.
void cSteamServer::OnP2PSessionConnectFail(P2PSessionConnectFail_t* pCallback)
{

	if (pCallback->m_eP2PSessionError != k_EP2PSessionErrorNone)
	{
		const char* description = "";

		switch (pCallback->m_eP2PSessionError)
		{
		case k_EP2PSessionErrorNone:
			description = "";
			break;
		case k_EP2PSessionErrorNotRunningApp:
			description = "";
			break;
		case k_EP2PSessionErrorNoRightsToApp:
			description = "";
			break;
		case k_EP2PSessionErrorDestinationNotLoggedIn:
			description = "";
			break;
		case k_EP2PSessionErrorTimeout:
			description = "User timed out";
			break;
		case k_EP2PSessionErrorMax:
			description = "";
			break;
		default:
			break;
		}
	}
}