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


class CSteamLobbySystem
{

	static const FriendsListMenuItem_t k_menuItemEmpty;

public:
	CSteamLobbySystem() {}
	virtual ~CSteamLobbySystem() {}

	/* This is for debugging we can call this to output a list of current user's friends */
	void CreateFriendsList();


protected:


	/* This lets us add friends with tags, Tags could be Friends list or pending, look at enum EFriendFlags in isteamfriends.h file to get more of idea */
	void AddFriends(int iFriendFlag, const char *pszName, std::vector<CSteamID> *pVecIgnoredSteamIds = NULL);


	/* this function could be use to added user's friends to a list/ in game menu. Need to think of better way to create this. */
	void AddFriendsToMenu(CSteamID steamIDFriend);




		
	
};