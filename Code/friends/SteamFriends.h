//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.h
//  Created:     23/09/2019 by Von Bismarck, Jonathan Green
//  Description: This is created and use to gather info on friends
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////




#pragma once 

#include "StdAfx.h"
#include <steam_api.h>

// a friends list item
struct FriendsListMenuItem_t
{
	CSteamID m_steamIDFriend;
};


class CFriendsList 
{

};


class SteamFriendsSystem : public CFriendsList
{
	static const FriendsListMenuItem_t k_menuItemEmpty;

public:
	SteamFriendsSystem();
	virtual ~SteamFriendsSystem() {}

	/*
	Friends
	*/



	/* This is for debugging we can call this to output a list of current user's friends */
	void CreateFriendsList();


protected:
	/* This lets us add friends with tags, Tags could be Friends list or pending, look at enum EFriendFlags in isteamfriends.h file to get more of idea */
	void AddFriends(int iFriendFlag, const char *pszName, std::vector<CSteamID> *pVecIgnoredSteamIds = NULL);


	/* this function could be use to added user's friends to a list/ in game menu. Need to think of better way to create this. */
	void AddFriendsToMenu(CSteamID steamIDFriend);


private:

	CFriendsList * m_pFriendsList;
};

