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
//#include <CryLobby/CommonICryLobby.h>
#include "public/steam/steam_api.h"
#include "public/steam/steam_gameserver.h"
/*Von will create the include in cmake*/
#include <../../CryPlugins/CryLobby/Module/CryLobby.h>
struct CryUserID;
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
class SteamLobbyService : public CCryLobbyService
{
public:
	SteamLobbyService(CCryLobby* pLobby, ECryLobbyService service);
	virtual ~SteamLobbyService(void);

	virtual ECryLobbyError   Initialise(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB);
	virtual ECryLobbyError   Terminate(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB);
	virtual void             Tick(CTimeValue tv);
	virtual ICryMatchMaking* GetMatchMaking();
	virtual CryUserID      GetUserID(uint32 user) { return CryUserInvalidID; }
	virtual ECryLobbyError GetSystemTime(uint32 user, SCrySystemTime* pSystemTime);

	virtual void           OnPacket(const TNetAddress& addr, CCryLobbyPacket* pPacket);
	virtual void           OnError(const TNetAddress& addr, ESocketError error, CryLobbySendID sendID);
	virtual void           OnSendComplete(const TNetAddress& addr, CryLobbySendID sendID);

	virtual void           GetSocketPorts(uint16& connectPort, uint16& listenPort);

	void                   InviteAccepted(uint32 user, CrySessionID id);

private:
#if !defined(RELEASE)
	AppId_t m_SteamAppID;
#endif // !defined(RELEASE)
};
class CCryLobby;
// an item in the list of lobbies we've found to display
struct Lobby_t
{
	CSteamID m_steamIDLobby;
	char m_rgchName[256];
};
typedef void(*CryLobbyServiceCallback)(ECryLobbyError error, CCryLobby* arg, ECryLobbyService service);
class SteamLobbySystem
{


public:
	ECryLobbyError Initialise(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB);
	~SteamLobbySystem(void);
	SteamLobbySystem();
	inline ECryLobbyError SteamLobbySystem::Initialise(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB)
	{
		return ECryLobbyError();
	}
	virtual ~SteamLobbySystem() {}

	inline SteamLobbySystem::SteamLobbySystem()
	{
	}

	inline void SteamLobbySystem::SetLobbySteamID(const CSteamID & steamIDLobby)
	{
	}

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
