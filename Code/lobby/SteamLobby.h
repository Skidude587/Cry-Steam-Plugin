/*


	This Class file is for calling Steam API for our lobby system



	Authors: Von Bismarck , 





*/



#pragma once 
#include "StdAfx.h"

#include "public/steam/steam_api.h"

// a friends list item
struct FriendsListMenuItem_t
{
	CSteamID m_steamIDFriend;
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


class CSteamLobbySystem
{

	static const FriendsListMenuItem_t k_menuItemEmpty;

public:
	CSteamLobbySystem() {}
	virtual ~CSteamLobbySystem() {}


	/*
		Lobby
	*/
	void SetLobbySteamID(const CSteamID &steamIDLobby);




	/*
		Friends
	*/

	/* This is for debugging we can call this to output a list of current user's friends */
	void CreateFriendsList();

	void CreateLobby();

	void RefreshLobby();




protected:


	/* This lets us add friends with tags, Tags could be Friends list or pending, look at enum EFriendFlags in isteamfriends.h file to get more of idea */
	void AddFriends(int iFriendFlag, const char *pszName, std::vector<CSteamID> *pVecIgnoredSteamIds = NULL);


	/* this function could be use to added user's friends to a list/ in game menu. Need to think of better way to create this. */
	void AddFriendsToMenu(CSteamID steamIDFriend);



private:
	/* Create the stuff that needs to private below since CSteamLobby would only need it */

	CSteamID m_steamIDLobby;

	// user state change handler
	STEAM_CALLBACK(CSteamLobbySystem, OnPersonaStateChange, PersonaStateChange_t, m_CallbackPersonaStateChange);

	// lobby state change handler
	STEAM_CALLBACK(CSteamLobbySystem, OnLobbyDataUpdate, LobbyDataUpdate_t, m_CallbackLobbyDataUpdate);
	STEAM_CALLBACK(CSteamLobbySystem, OnLobbyChatUpdate, LobbyChatUpdate_t, m_CallbackChatDataUpdate);
	
};