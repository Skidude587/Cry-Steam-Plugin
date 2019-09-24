//
// -------------------------------------------------------------------------
//  File name:   SteamServer.cpp
//  Created:     23/09/2019 by Joey Gonzalez, Jonathan Green
//  Description: Handles P2P sessions for sending/recieving packets
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "SteamServer.h"

// Wrapper function to check if theres a packet ready to be read.
bool cSteamServer::IsP2PPacketAvailable(uint32* pMsgSize)
{
	return SteamNetworking()->IsP2PPacketAvailable(pMsgSize);
}

// Recieve a packet along with its size and the associated user's steamID.
// Returns false if there is no packet available to read, make sure to call
// IsP2PPacketAvailable() prior to ReadP2PPacket. Returns true if a packet
// Is sucessfully read.
bool cSteamServer::ReadP2PPacket(void* pDest, uint32 destSize, uint32* pMsgSize, CSteamID* pSteamIDRemote)
{
	return SteamNetworking()->ReadP2PPacket(pDest, destSize, pMsgSize, pSteamIDRemote);
}

// Close P2P session with associated steamID.
void cSteamServer::CloseP2PSession(CSteamID steamIDRemote)
{
	if (!SteamNetworking()->CloseP2PSessionWithUser(steamIDRemote))
	{
		// No connection ever opened on that steamID.
		CRY_ASSERT("CloseP2PSession() no connection opened on steamID[%");
	}
}

// Callback for recieving P2P requests.
void cSteamServer::OnP2PSessionRequest(P2PSessionRequest_t* pCallback)
{
	// If returned false, steamID is invalid. 
	if (!SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		// SteamID is invalid.
		CRY_ASSERT("OnP2PSessionRequest() ");
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
		CRY_ASSERT("SteamNetworking not working!");
	}
}