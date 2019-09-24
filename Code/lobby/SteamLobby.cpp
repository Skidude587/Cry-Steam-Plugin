//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.cpp
//  Created:     23/09/2019 by Jonathan Green, 
//  Description: Creating the Multiplayer Steam Lobby
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SteamLobby.h"


#if !defined(RELEASE)
//Steam Friend service
static void SteamShowFriends_DevelopmentOnly(IConsoleCmdArgs* pArgs)
{
	ISteamFriends* pSteamFriends = SteamFriends();
	if (!pSteamFriends)
	{
		/*Warning*/(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Steam friends service not available");
		return;
	}
	uint32 friendCount = pSteamFriends->GetFriendCount(k_EFriendFlagImmediate);
	/*Log*/("[STEAM]: Friends list (%d friends):", friendCount);
	for (uint32 index = 0; index < friendCount; ++index)
	{
		CSteamID friendID = pSteamFriends->GetFriendByIndex(index, k_EFriendFlagImmediate);
		/*Need to creating logging*/
	}
}
//

//Steam overlay
	static void SteamShowOverlay_DevelopmentOnly(IConsoleCmdArgs* pArgs)
	{
		ISteamUtils* pSteamUtils = SteamUtils();
		ISteamFriends* pSteamFriends = SteamFriends();
		if (!pSteamUtils || !pSteamFriends)
		{
			/*Warning*/(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Overlay cannot be shown; Steam is not initialized");
			return;
		}


		const char* overlay[] = { "Friends", "Community", "Players", "Settings", "OfficialGameGroup", "Stats", "Achievements", "LobbyInvite" };
		uint32 numOverlays = sizeof(overlay) / sizeof(char*);
		uint32 requestedOverlay = 0;

		if (pArgs->GetArgCount() > 1)
		{
			requestedOverlay = atoi(pArgs->GetArg(1));
			if ((requestedOverlay < 0) || (requestedOverlay > numOverlays))
			{
				/*Log*/("Illegal overlay index specified [%d]:", requestedOverlay);
				for (uint32 index = 0; index < numOverlays; ++index)
				{
					/*Log*/("  [%d] %s", index, overlay[index]);
				}
				return;
			}
		}

		if (pSteamUtils->IsOverlayEnabled() == false)
		{
			/*Log*/("[STEAM]: Attempting to show [%s] overlay", overlay[requestedOverlay]);
			pSteamFriends->ActivateGameOverlay(overlay[requestedOverlay]);
		}
		else
		{
			/*Log*/("[STEAM]: overlay already displayed");
		}
	}
//

//Steam Service Int
/*steam service int here*/
/*steam appid int here*/
/*steam accound id here*/
/*CSteamID int here*/
//

//Steam Stats upload
/*steam stats upload here*/
//
#endif
