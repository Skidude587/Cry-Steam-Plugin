//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.h
//  Created:     23/09/2019 by Von Bismarck, Jonathan Green
//  Description: This Class file is for calling Steam API for our lobby system
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////




#pragma once 
#include "StdAfx.h"
#include <CryLobby/CommonICryLobby.h>
#include "public/steam/steam_api.h"
#include "public/steam/steam_gameserver.h"
#include <../../CryEngine/CryCommon/CrySystem/ISystem.h>

#define CRYLOBBY_USER_NAME_LENGTH        32
#define CRYLOBBY_USER_GUID_STRING_LENGTH 40

struct SCryUserID : public CMultiThreadRefCount
{
	virtual bool                                              operator==(const SCryUserID& other) const = 0;
	virtual bool                                              operator<(const SCryUserID& other) const = 0;

	virtual CryFixedStringT<CRYLOBBY_USER_GUID_STRING_LENGTH> GetGUIDAsString() const
	{
		return CryFixedStringT<CRYLOBBY_USER_GUID_STRING_LENGTH>("");
	}
};
#define TMemHdl                     LobbyTMemHdl
typedef void* TMemHdl;
#define TMemInvalidHdl NULL

class CCryLobby;

struct SCrySharedUserID : public SCryUserID
{
	void* operator new(size_t size) throw()
	{
		CCryLobby* pLobby = (CCryLobby*)CCryLobby::GetLobby();

		if (pLobby)
		{
			TMemHdl hdl = pLobby->MemAlloc(size);

			if (hdl != TMemInvalidHdl)
			{
				SCrySharedUserID* id = (SCrySharedUserID*)pLobby->MemGetPtr(hdl);

				id->hdl = hdl;

				return id;
			}
		}

		return NULL;
	}

	void operator delete(void* p)
	{
		if (p)
		{
			CCryLobby* pLobby = (CCryLobby*)CCryLobby::GetLobby();

			if (pLobby)
			{
				pLobby->MemFree(((SCrySharedUserID*)p)->hdl);
			}
		}
	}

	TMemHdl hdl;
};

#define STEAMID_AS_STRING_LENGTH (32)
static CryFixedStringT<STEAMID_AS_STRING_LENGTH> CSteamIDAsString(const CSteamID& id)
{
	CryFixedStringT<STEAMID_AS_STRING_LENGTH> result;
	result.Format("%02X:%X:%05X:%08X", id.GetEUniverse(), id.GetEAccountType(), id.GetUnAccountInstance(), id.GetAccountID());

	return result;
};

struct SCryLobbyDedicatedServerSetupData
{
	CCryLobbyPacket* pPacket;
	CrySessionHandle session;
};

struct SteamUserID : public SCrySharedUserID
{
	SteamUserID()
		: m_steamID(CSteamID())
	{}

	SteamUserID(const CSteamID& steamID)
		: m_steamID(steamID)
	{}

	virtual bool operator==(const SCryUserID& other) const
	{
		return (m_steamID == static_cast<const SteamUserID&>(other).m_steamID);
	}

	virtual bool operator<(const SCryUserID& other) const
	{
		return (m_steamID < static_cast<const SteamUserID&>(other).m_steamID);
	}

	virtual CryFixedStringT<CRYLOBBY_USER_GUID_STRING_LENGTH> GetGUIDAsString() const
	{
		CryFixedStringT<CRYLOBBY_USER_GUID_STRING_LENGTH> result(CSteamIDAsString(m_steamID));
		return result;
	};

	void WriteToPacket(CCryLobbyPacket* pPacket) const
	{
		pPacket->WriteUINT64(m_steamID.ConvertToUint64());
	}

	void ReadFromPacket(CCryLobbyPacket* pPacket)
	{
		m_steamID.SetFromUint64(pPacket->ReadUINT64());
	}

	SteamUserID& operator=(const SteamUserID& other)
	{
		m_steamID = other.m_steamID;
		return *this;
	}

	CSteamID m_steamID;
};

struct LobbyMenuItem_t
{
	/* Probably need to change this wording for enum.... */
	enum ELobbyMenuCommand
	{
		k_ELobbyMenuItemUser,
		k_ELobbyMenuItemStartGame,
		k_ELobbyMenuItemToggleReadState,
		k_ELobbyMenuItemLeaveLobby,
		k_ELobbyMenuItemInviteToLobby
	};

	CSteamID m_steamIDUser;		// the user who this is in the lobby
	ELobbyMenuCommand m_eCommand;
	CSteamID m_steamIDLobby;	// set if k_ELobbyMenuItemInviteToLobby	
};

// an item in the list of lobbies we've found to display
struct Lobby_t
{
	CSteamID m_steamIDLobby;
	char m_rgchName[256];
};

typedef void(*CryLobbyServiceCallback)(ECryLobbyError error, CCryLobby* arg, ECryLobbyService service);

class CSteamLobbySystem
{


public:
	ECryLobbyError Initialise(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB);
	~CSteamLobbySystem(void);
	CSteamLobbySystem();
	inline ECryLobbyError CSteamLobbySystem::Initialise(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB)
	{
		return ECryLobbyError();
	}
	virtual ~CSteamLobbySystem() {}

	/*
		Lobby
	*/
	void SetLobbySteamID(const CSteamID &steamIDLobby);


protected:


	



private:
	/* Create the stuff that needs to private below since CSteamLobby would only need it */

	CSteamID m_steamIDLobby;

	// user state change handler
	STEAM_CALLBACK(CSteamLobbySystem, OnPersonaStateChange, PersonaStateChange_t, m_CallbackPersonaStateChange);

	// lobby state change handler
	STEAM_CALLBACK(CSteamLobbySystem, OnLobbyDataUpdate, LobbyDataUpdate_t, m_CallbackLobbyDataUpdate);
	STEAM_CALLBACK(CSteamLobbySystem, OnLobbyChatUpdate, LobbyChatUpdate_t, m_CallbackChatDataUpdate);
	
};