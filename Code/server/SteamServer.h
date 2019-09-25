//
// -------------------------------------------------------------------------
//  File name:   SteamServer.h
//  Created:     23/09/2019 by Joey Gonzalez, Jonathan Green
//  Description: Setup for P2P sessions and IServer
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#pragma once 
#include "StdAfx.h"
#include <steam_api.h>
#include <steamclientpublic.h>

//Identifier
//! Simple opaque class used to store a service-specific identifier.
template <typename Traits>

class Identifier
{
	using ValueType = typename Traits::ValueType;
	using ServiceType = typename Traits::ServiceType;
public:
	Identifier(const ServiceType& svcId, const ValueType& accountId)
		: m_svcId(svcId)
		, m_value(accountId)
	{
	}
	inline AccountIdentifier     CreateAccountIdentifier(AccountIdentifierValue rawSteamId) { return AccountIdentifier(SteamServiceID, rawSteamId); }
	inline LobbyIdentifier       CreateLobbyIdentifier(LobbyIdentifierValue rawSteamId) { return LobbyIdentifier(SteamServiceID, rawSteamId); }
	inline ApplicationIdentifier CreateApplicationIdentifier(ApplicationIdentifierValue rawSteamId) { return ApplicationIdentifier(SteamServiceID, rawSteamId); }

	inline AccountIdentifier     CreateAccountIdentifier(const CSteamID& steamId) { return CreateAccountIdentifier(steamId.ConvertToUint64()); }
	inline LobbyIdentifier       CreateLobbyIdentifier(const CSteamID& steamId) { return CreateLobbyIdentifier(steamId.ConvertToUint64()); }

	Identifier() = default;

	Identifier(const Identifier&) = default;
	Identifier(Identifier&&) = default;

	Identifier& operator=(const Identifier&) = default;
	Identifier& operator=(Identifier&&) = default;

	const ServiceType& Service() const { return m_svcId; }

	bool operator==(const Identifier& other) const { return std::tie(m_svcId, m_value) == std::tie(other.m_svcId, other.m_value); }
	bool operator!=(const Identifier& other) const { return std::tie(m_svcId, m_value) != std::tie(other.m_svcId, other.m_value); }
	bool operator<(const Identifier& other) const { return std::tie(m_svcId, m_value) < std::tie(other.m_svcId, other.m_value); }

	bool GetAsUint64(uint64& out) const
	{
		const bool success = Traits::AsUint64(m_value, out);
		if (!success)
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_COMMENT, "[GamePlatform] %s: Unable to convert '%s'.", __func__, ToDebugString());
		}

		return success;
	}

	template<class StrType>
	bool GetAsString(StrType& out) const
	{
		const bool success = Traits::AsString(m_value, out);
		if (!success)
		{
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_COMMENT, "[GamePlatform] %s: Unable to convert '%s'.", __func__, ToDebugString());
		}

		return success;
	}

	void Serialize(Serialization::IArchive& ar)
	{
		ar(m_svcId, "service");

		return Traits::Serialize(m_value, ar);
	}

	const char* ToDebugString() const
	{
		return Traits::ToDebugString(m_svcId, m_value);
	}

private:
	ServiceType m_svcId = CryGUID::Null();
	ValueType m_value = Traits::Null();
};

//Account Identifier
//! Identifies a game platform user on a specific service.
using AccountIdentifier = Identifier<SAccountTraits>;

//! Identifies a game platform lobby.
using LobbyIdentifier = Identifier<SLobbyTraits>;

//! Identifies a game or DLC.
using ApplicationIdentifier = Identifier<SApplicationTraits>;

//! Identifies a game platform user on a specific service.
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



using ServiceIdentifier = CryGUID;

constexpr ServiceIdentifier NullServiceID = CryGUID::Null();
constexpr ServiceIdentifier SteamServiceID = CryClassID("{4DFCDCE3-9985-42E5-A702-5A64D849DC8F}"_cry_guid);
constexpr ServiceIdentifier PSNServiceID = CryClassID("{F7729E26-464F-4BE9-A58E-06C72C03EAE1}"_cry_guid);
constexpr ServiceIdentifier DiscordServiceID = CryClassID("{D68238FE-AA88-4C0C-9E9C-56A848AE0F37}"_cry_guid);
constexpr ServiceIdentifier RailServiceID = CryClassID("{B536B2AE-E363-4765-8E15-7B021C356E9C}"_cry_guid);
constexpr ServiceIdentifier XboxServiceID = CryClassID("{BE28931B-8843-419F-BDE4-C167F3EAAFBA}"_cry_guid);

inline const char* GetServiceDebugName(const ServiceIdentifier& svcId)
{
	return (svcId == SteamServiceID) ? "Steam" :
		((svcId == PSNServiceID) ? "PSN" :
		((svcId == XboxServiceID) ? "Xbox" :
			((svcId == DiscordServiceID) ? "Discord" :
			((svcId == RailServiceID) ? "Rail" :
				((svcId == NullServiceID) ? "Null" : "Unknown")))));
}


class CSteamServer
	: public IServer
{
public:
	void CloseP2PSession(CSteamID steamIDRemote);
	bool IsP2PPacketAvailable(uint32* pMsgSize);
	bool ReadP2PPacket(void* pDest, uint32 destSize, uint32* pMsgSize, CSteamID* pSteamIDRemote);
	
	explicit CSteamServer(bool bLocal);
	virtual ~CSteamServer();

	// IServer
	virtual IServer::Identifier GetIdentifier() const override;

	virtual uint32 GetPublicIP() const override;
	virtual const char* GetPublicIPString() const override;

	virtual uint16 GetPort() const override;

	virtual bool IsLocal() const override { return m_bLocal; }

	virtual bool AuthenticateUser(uint32 clientIP, char* authData, int authDataLength, AccountIdentifier &userId) /*override*/
	{
		if (ISteamGameServer* pGameServer = SteamGameServer())
		{
			CSteamID steamUserId;
			if (pGameServer->SendUserConnectAndAuthenticate(clientIP, authData, authDataLength, &steamUserId))
			{
				userId = Detail::CreateAccountIdentifier(steamUserId);
				return true;
			}
			else
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] Steam authentication failure!");
		}
		else
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "[Steam] No game server found, returning authentication failure!");

		return false;
	}
	virtual void SendUserDisconnect(const AccountIdentifier& userId) /*override*/;
public:
	STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t);
	STEAM_CALLBACK(cSteamServer, OnP2PSessionConnectFail, P2PSessionConnectFail_t);
protected:
		bool m_bLocal;
};