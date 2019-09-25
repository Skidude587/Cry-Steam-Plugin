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
using AccountIdentifier = Identifier<Detail::SAccountTraits>;

//! Identifies a game platform lobby.
using LobbyIdentifier = Identifier<Detail::SLobbyTraits>;

//! Identifies a game or DLC.
using ApplicationIdentifier = Identifier<Detail::SApplicationTraits>;

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

namespace Detail {
	struct SAccountTraits : public STraitsBase
	{
		static const char* ToDebugString(const ServiceIdentifier& svcId, const ValueType& value)
		{
			return STraitsBase::ToDebugString(svcId, "Account", value);
		}
	};

	struct SLobbyTraits : public STraitsBase
	{
		static const char* ToDebugString(const ServiceIdentifier& svcId, const ValueType& value)
		{
			return STraitsBase::ToDebugString(svcId, "Lobby", value);
		}
	};

	struct SApplicationTraits : public STraitsBase
	{
		static const char* ToDebugString(const ServiceIdentifier& svcId, const ValueType& value)
		{
			return STraitsBase::ToDebugString(svcId, "Application", value);
		}
	};

	using NumericIdentifierValue = uint64;

	//Traits Base
	struct STraitsBase
	{
		using ServiceType = ServiceIdentifier;
		using NumericIdentifierValue = uint64;
		using StringIdentifierValue = CryFixedStringT<48>;
		// Note: When adding types here make sure you update the code using stl::holds_alternative
		// and stl::get
		using ValueType = CryVariant<StringIdentifierValue, NumericIdentifierValue>;

		static ValueType Null()
		{
			return NumericIdentifierValue(0);
		}

		static const char* ToDebugString(const ServiceIdentifier& svcId, const char* szIdKind, const ValueType& value)
		{
			static stack_string debugStr;

			if (stl::holds_alternative<StringIdentifierValue>(value))
			{
				debugStr.Format("%s%s:%s", GetServiceDebugName(svcId), szIdKind, stl::get<StringIdentifierValue>(value).c_str());
			}
			else if (stl::holds_alternative<NumericIdentifierValue>(value))
			{
				debugStr.Format("%s%s:%" PRIu64, GetServiceDebugName(svcId), szIdKind, stl::get<NumericIdentifierValue>(value));
			}
			else
			{
				return debugStr.Format("%s%s:?", GetServiceDebugName(svcId), szIdKind);
			}

			return debugStr.c_str();
		}

		static bool AsUint64(const ValueType& value, uint64& out)
		{
			if (stl::holds_alternative<StringIdentifierValue>(value))
			{
				char trailing; // attempt to parse trailing characters as we don't want them.
				const StringIdentifierValue& str = stl::get<StringIdentifierValue>(value);
				const int ok = sscanf_s(str.c_str(), "%" PRIu64 "%c", &out, &trailing);
				return ok == 1;
			}
			else if (stl::holds_alternative<NumericIdentifierValue>(value))
			{
				out = stl::get<NumericIdentifierValue>(value);
				return true;
			}

			return false;
		}

		template<class StrType>
		static bool AsString(const ValueType& value, StrType& out)
		{
			if (stl::holds_alternative<StringIdentifierValue>(value))
			{
				out = stl::get<StringIdentifierValue>(value).c_str();
				return true;
			}
			else if (stl::holds_alternative<NumericIdentifierValue>(value))
			{
				out.Format("%" PRIu64, stl::get<NumericIdentifierValue>(value));
				return true;
			}
			return false;
		}

		static void Serialize(ValueType& value, Serialization::IArchive& ar)
		{
			constexpr size_t strIdx = cry_variant::get_index<StringIdentifierValue, ValueType>::value;
			constexpr size_t numIdx = cry_variant::get_index<NumericIdentifierValue, ValueType>::value;

			if (ar.isOutput())
			{
				const size_t idx = value.index();
				switch (idx)
				{
				case strIdx:
					ar(idx, "type");
					ar(stl::get<StringIdentifierValue>(value), "value");
					break;
				case numIdx:
					ar(idx, "type");
					ar(stl::get<NumericIdentifierValue>(value), "value");
					break;
				default:
					ar(stl::variant_npos, "type");
				}

				return;
			}

			if (ar.isInput())
			{
				ValueType tmpVal;

				size_t idx = stl::variant_npos;
				ar(idx, "type");

				switch (idx)
				{
				case strIdx:
				{
					StringIdentifierValue str;
					ar(str, "value");
					tmpVal = str;
				}
				break;
				case numIdx:
				{
					NumericIdentifierValue num;
					ar(num, "value");
					tmpVal = num;
				}
				break;
				}

				if (tmpVal.index() != idx)
				{
					CRY_ASSERT(tmpVal.index() == idx, "Variant deserialization failed!");
					return;
				}

				value.swap(tmpVal);
			}
		}
	};
	inline CSteamID ExtractSteamIDAccount(const AccountIdentifier& accountId)
	{
		if (accountId.Service() == SteamServiceID)
		{
			AccountIdentifierValue tmpVal;
			if (accountId.GetAsUint64(tmpVal))
			{
				return tmpVal;
			}
		}

		CRY_ASSERT(false, "[Steam] AccountIdentifier '%s' does not seem to contain a valid Steam ID", accountId.ToDebugString());
		return k_steamIDNil;
	}

	inline CSteamID ExtractSteamIDLobby(const LobbyIdentifier& lobbyId)
	{
		if (lobbyId.Service() == SteamServiceID)
		{
			LobbyIdentifierValue tmpVal;
			if (lobbyId.GetAsUint64(tmpVal))
			{
				return tmpVal;
			}
		}

		CRY_ASSERT(false, "[Steam] LobbyIdentifier '%s' does not seem to contain a valid Steam ID", lobbyId.ToDebugString());
		return k_steamIDNil;
	}

	inline AppId_t ExtractSteamID(const ApplicationIdentifier& appId)
	{
		if (appId.Service() == SteamServiceID)
		{
			ApplicationIdentifierValue tmpVal;
			if (appId.GetAsUint64(tmpVal))
			{
				if (tmpVal <= std::numeric_limits<AppId_t>::max())
				{
					return static_cast<AppId_t>(tmpVal);
				}

				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_COMMENT, "[GamePlatform] %s: Unable to convert '%s' to AppId_t.", __func__, appId.ToDebugString());
			}
		}

		CRY_ASSERT(false, "[Steam] ApplicationIdentifier '%s' does not seem to contain a valid Steam ID", appId.ToDebugString());
		return k_uAppIdInvalid;
	}
	

	using AccountIdentifierValue = Detail::NumericIdentifierValue;
	using LobbyIdentifierValue = Detail::NumericIdentifierValue;
	using ApplicationIdentifierValue = Detail::NumericIdentifierValue;

	inline AccountIdentifier     CreateAccountIdentifier(AccountIdentifierValue rawSteamId) { return AccountIdentifier(SteamServiceID, rawSteamId); }
	inline LobbyIdentifier       CreateLobbyIdentifier(LobbyIdentifierValue rawSteamId) { return LobbyIdentifier(SteamServiceID, rawSteamId); }
	inline ApplicationIdentifier CreateApplicationIdentifier(ApplicationIdentifierValue rawSteamId) { return ApplicationIdentifier(SteamServiceID, rawSteamId); }

	inline AccountIdentifier     CreateAccountIdentifier(const CSteamID& steamId) { return CreateAccountIdentifier(steamId.ConvertToUint64()); }
	inline LobbyIdentifier       CreateLobbyIdentifier(const CSteamID& steamId) { return CreateLobbyIdentifier(steamId.ConvertToUint64()); }
}

class cSteamServer
	: public IServer
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
private:
	STEAM_CALLBACK(cSteamServer, OnP2PSessionRequest, P2PSessionRequest_t);
	STEAM_CALLBACK(cSteamServer, OnP2PSessionConnectFail, P2PSessionConnectFail_t);
protected:
		bool m_bLocal;
};