//
// -------------------------------------------------------------------------
//  File name:   SteamServer.h
//  Created:     23/09/2019 by Joey Gonzalez, Jonathan Green
//  Description: Class header for P2P sessions
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#pragma once 
#include "StdAfx.h"
#include <../../CryGamePlatform/Interface/IPlatformServer.h>
#include <../../CryPlugins/CryGamePlatform/Module/Steam/SteamTypes.h>
#include "PlatformIdentifier.h"

class cSteamServer
{
public:
	void CloseP2PSession(CSteamID steamIDRemote);
	bool IsP2PPacketAvailable(uint32* pMsgSize);
	bool ReadP2PPacket(void* pDest, uint32 destSize, uint32* pMsgSize, CSteamID* pSteamIDRemote);
	
	explicit cSteamServer(bool bLocal);
	virtual ~cSteamServer();

	// IServer
	virtual IServer::Identifier GetIdentifier() const override;

	virtual uint32 GetPublicIP() const override;
	virtual const char* GetPublicIPString() const override;

	virtual uint16 GetPort() const override;

	virtual bool IsLocal() const override { return m_bLocal; }

	virtual bool AuthenticateUser(uint32 clientIP, char* authData, int authDataLength, AccountIdentifier &userId) override
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
		{
			CSteamID steamUserId;
			if (pGameServer->SendUserConnectAndAuthenticate(clientIP, authData, authDataLength, &steamUserId))
			{
				userId = CreateAccountIdentifier(steamUserId);
				return true;
			}
			else
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] Steam authentication failure!");
		}
		else
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] No game server found, returning authentication failure!");

		return false;
	}
	virtual void SendUserDisconnect(const AccountIdentifier& userId) override;
	void cSteamServer::SendUserDisconnect(const AccountIdentifier& userId)
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
		{
			pGameServer->SendUserDisconnect(ExtractSteamID(userId));
		}
	}
private:
	STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t);
	STEAM_CALLBACK(cSteamServer, OnP2PSessionConnectFail, P2PSessionConnectFail_t);
protected:
		bool m_bLocal;
};

struct IServer
{
	// Unique identifier of the server
	using Identifier = uint64;

	virtual ~IServer() {}

	//! Gets the server's unique identifier
	virtual Identifier GetIdentifier() const = 0;

	//! Gets the public IP address of the server according to the platform's network
	virtual uint32 GetPublicIP() const = 0;
	//! Gets the public IP address of the server according to the platform's network as a string
	virtual const char* GetPublicIPString() const = 0;
	//! Gets the port that clients should connect to
	virtual uint16 GetPort() const = 0;
	//! Checks whether or not the server expects clients to connect with the server's local IP (LAN)
	virtual bool IsLocal() const = 0; 
	//! \returns True if the authentication was successful, otherwise false
	virtual bool AuthenticateUser(uint32 clientIP, char* szAuthData, int authDataLength, AccountIdentifier& userId) = 0;
	//! Should be called whenever a user leaves the game server, allowing the platform services to track what users are on which servers.
	virtual void SendUserDisconnect(const AccountIdentifier& userId) = 0;
};


