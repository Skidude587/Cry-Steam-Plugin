#include "StdAfx.h"

#include "SteamFriends.h"
#include <steam_api.h>
/*
	These Functions are open API, meaning you call them with in your game.
*/



SteamFriendsSystem::SteamFriendsSystem()
{
}

void SteamFriendsSystem::CreateFriendsList()
{
	AddFriends(k_EFriendFlagFriendshipRequested, "Incoming Friend Requests");
	std::vector<CSteamID> vecTaggedSteamIDs;
	int nFriendsGroups = SteamFriends()->GetFriendsGroupCount();

	for (int iFG = 0; iFG < nFriendsGroups; iFG++)
	{
		FriendsGroupID_t friendsGroupID = SteamFriends()->GetFriendsGroupIDByIndex(iFG);
		if (friendsGroupID == k_FriendsGroupID_Invalid)
			continue;

		int nFriendsGroupMemberCount = SteamFriends()->GetFriendsGroupMembersCount(friendsGroupID);
		if (!nFriendsGroupMemberCount)
			continue;

		const char *pszFriendsGroupName = SteamFriends()->GetFriendsGroupName(friendsGroupID);
		if (pszFriendsGroupName == NULL)
			pszFriendsGroupName = "";
		/*
		 We can generate friends list here.
		*/
		CryLog(pszFriendsGroupName);


		std::vector<CSteamID> vecSteamIDMembers(nFriendsGroupMemberCount);
		SteamFriends()->GetFriendsGroupMembersList(friendsGroupID, &vecSteamIDMembers[0], nFriendsGroupMemberCount);
		for (int iMember = 0; iMember < nFriendsGroupMemberCount; iMember++)
		{
			const CSteamID &steamIDMember = vecSteamIDMembers[iMember];
			
			CryLog("%s",steamIDMember);
			vecTaggedSteamIDs.push_back(steamIDMember);
		}
	}

	AddFriends(k_EFriendFlagImmediate, "Friends", &vecTaggedSteamIDs);
	AddFriends(k_EFriendFlagRequestingFriendship, "Outgoing Friend Request");


}



void SteamFriendsSystem::AddFriends(int iFriendFlag, const char * pszName, std::vector<CSteamID>* pVecIgnoredSteamIds)
{
	int iFriendCount = SteamFriends()->GetFriendCount(iFriendFlag);
	if (!iFriendCount)
		return;

	//AddMenuItem(CFriendsListMenu::MenuItem_t("", k_menuItemEmpty));
	//AddMenuItem(CFriendsListMenu::MenuItem_t(pszName, k_menuItemEmpty));
	CryLog(pszName);

	for (int iFriend = 0; iFriend < iFriendCount; iFriend++)
	{
		CSteamID steamIDFriend = SteamFriends()->GetFriendByIndex(iFriend, iFriendFlag);

		// This mimicks the Steam client's feature where it only shows
		// untagged friends in the canonical Friends section by default
		if (pVecIgnoredSteamIds && (std::find(pVecIgnoredSteamIds->begin(), pVecIgnoredSteamIds->end(), steamIDFriend) != pVecIgnoredSteamIds->end()))
			continue;

		//AddFriendToMenu(steamIDFriend);
		CryLog("%s",steamIDFriend);
	}
}

void SteamFriendsSystem::AddFriendsToMenu(CSteamID steamIDFriend)
{
	if (!steamIDFriend.IsValid())
		return;

	FriendsListMenuItem_t menuItemFriend = { steamIDFriend };

	char szFriendNameBuffer[512] = { '\0' };

	const char *pszFriendName = SteamFriends()->GetFriendPersonaName(steamIDFriend);
	CryLog(szFriendNameBuffer, "%s", pszFriendName);

	const char *pszFriendNickname = SteamFriends()->GetPlayerNickname(steamIDFriend);
	if (pszFriendNickname)
	{
		CryLog(szFriendNameBuffer, "%s (%s)", szFriendNameBuffer, pszFriendNickname);
	}

}
