//
//	File: SteamServer.cpp
//	
//
//

#include "StdAfx.h"
#include "SteamServer.h"
#include "lobby/steam_int_h.h"
#include "public/steam/steam_api.h"


// Close P2P session with associated steamID.
void cSteamServer::CloseP2PSession(CSteamID steamIDRemote)
{
	if (!SteamNetworking()->CloseP2PSessionWithUser(steamIDRemote))
	{
		// No connection ever opened on that steamID.
		// Add log ...

	}
}

// Callback for recieving P2P requests.
void cSteamServer::OnP2PSessionRequest(P2PSessionRequest_t* pCallback)
{
	// If returned false, steamID is invalid. 
	if (!SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		// Add error log ...

	}
}

// Callback for when a P2P connection fails.
void cSteamServer::OnP2PSessionConnectFail(P2PSessionConnectFail_t* pCallback)
{
	// If no error, ignore.
	if (pCallback->m_eP2PSessionError != k_EP2PSessionErrorNone)
	{
		const char* description = "";

		switch (pCallback->m_eP2PSessionError)
		{
		case k_EP2PSessionErrorNotRunningApp:
			description = "User isn't running game";
			break;
		case k_EP2PSessionErrorNoRightsToApp:
			description = "User doesn't own game";
			break;
		case k_EP2PSessionErrorDestinationNotLoggedIn:
			description = "User not connected to Steam";
			break;
		case k_EP2PSessionErrorTimeout:
			description = "User timed out";
			break;
		}

		// Log error ...

	}
}