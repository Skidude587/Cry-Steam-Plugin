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

// TODO: add log calls

// Check if theres a packet ready to be read.
bool cSteamServer::IsP2PPacketAvailable(uint32* pMsgSize)
{
	if (!SteamNetworking()->IsP2PPacketAvailable(pMsgSize))
	{
		// Add error log ...
		CRY_ASSERT("SteamNetworking not working!");
		return false;
	}

	return SteamNetworking()->IsP2PPacketAvailable(pMsgSize);
}

// Recieve a packet along with its size and the associated user's steamID.
// Returns false if there is no packet available to read, make sure to call
// IsP2PPacketAvailable() prior to ReadP2PPacket. Returns true if a packet
// Is sucessfully read.
bool cSteamServer::ReadP2PPacket(void* pDest, uint32 destSize, uint32* pMsgSize, CSteamID* pSteamIDRemote)
{
	if (!SteamNetworking()->ReadP2PPacket(pDest, destSize, pMsgSize, pSteamIDRemote))
	{
		// Add error log ...
		CRY_ASSERT("SteamNetworking not working!");
		return false;
	}

	return SteamNetworking()->ReadP2PPacket(pDest, destSize, pMsgSize, pSteamIDRemote);
}

cSteamServer::STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t)
{
}

cSteamServer::STEAM_CALLBACK(cSteamServer, OnP2PSessionConnectFail, P2PSessionConnectFail_t)
{
}

// Close P2P session with associated steamID.
void cSteamServer::CloseP2PSession(CSteamID steamIDRemote)
{
	if (!SteamNetworking()->CloseP2PSessionWithUser(steamIDRemote))
	{
		// Add error log ...
		CRY_ASSERT("SteamNetworking not working!");
		return;
	}

	if (!SteamNetworking()->CloseP2PSessionWithUser(steamIDRemote))
	{
		// No connection ever opened on that steamID.
		// Add log ...

	}
}

// Callback for recieving P2P requests.
void cSteamServer::OnP2PSessionRequest(P2PSessionRequest_t* pCallback)
{
	if (!SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		// Add error log ...
		CRY_ASSERT("SteamNetworking not working!");
		return;
	}

	// If returned false, steamID is invalid. 
	if (!SteamNetworking()->AcceptP2PSessionWithUser(pCallback->m_steamIDRemote))
	{
		// SteamID is invalid.
		// Add error log ...
		CRY_ASSERT("SteamNetworking not working!");
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