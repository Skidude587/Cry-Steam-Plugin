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
#include <public/steam/steam_gameserver.h>

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
	uint32 cSteamServer::GetPublicIP() const
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
			return pGameServer->GetPublicIP();

		return 0;
		const char* cSteamServer::GetPublicIPString() const
		{
			uint32 publicIP = GetPublicIP();

			const int NBYTES = 4;
			uint8 octet[NBYTES];
			char* ipAddressFinal = new char[15];
			for (int i = 0; i < NBYTES; i++)
			{
				octet[i] = publicIP >> (i * 8);
			}
			cry_sprintf(ipAddressFinal, NBYTES, "%d.%d.%d.%d", octet[3], octet[2], octet[1], octet[0]);

			string sIP = string(ipAddressFinal);
			delete[] ipAddressFinal;

			return sIP;
		};
		void cSteamServer::SendUserDisconnect(const AccountIdentifier& userId)
		{
			if (ISteamGameServer* pGameServer = SteamGameServer())
			{
				pGameServer->SendUserDisconnect(ExtractSteamID(userId));
			}
		};
	};

	IServer::Identifier cSteamServer::GetIdentifier() const
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
			return pGameServer->GetSteamID().ConvertToUint64();

		return 0;
	};

	uint32 cSteamServer::GetPublicIP() const
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
			return pGameServer->GetPublicIP();

		return 0;
	};

	uint16 cSteamServer::GetPort() const
	{
		ICVar* pPortVar = gEnv->pConsole->GetCVar("cl_serverport");

		return pPortVar->GetIVal();
	}
	inline void cSteamServer::SendUserDisconnect(const AccountIdentifier & userId)
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
		{
			pGameServer->SendUserDisconnect(ExtractSteamID(userId));
		}
	}
	cSteamServer::STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t)
	{
	}
	;

