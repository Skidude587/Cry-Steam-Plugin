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
#include <../../CryPlugins/CryLobby/Module/CryLobby.h>


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


class CSteamLobbySystem
{


public:
	CSteamLobbySystem();
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