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
#include <CrySystem/ConsoleRegistration.h>
#include <../../CryPlugins/CryGamePlatform/Interface/IPlatformService.h>

#if RELEASE
static void SteamInviteToGame_DevelopmentOnly(IConsoleCmdArgs* pArgs)
{
	ISteamFriends* pSteamFriends = SteamFriends();
	if (!pSteamFriends)
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Steam friends service not available");
		return;
	}
	uint32 friendCount = pSteamFriends->GetFriendCount(k_EFriendFlagImmediate);
	uint32 argCount = pArgs->GetArgCount();

	if (argCount > 1)
	{
		uint32 argIndex = 1;
		/*SteamLobbyService**/ /*LobbyService*/ = (/*SteamLobbyService**/)gEnv->pLobby->/*GetLobbyService(eCLS_Online)*/;
		CryLobbySessionHandle lsh = ((CCryMatchMaking*)pLobbyService->GetMatchMaking())->GetCurrentHostedNetNubSessionHandle();

		if (lsh != CryLobbyInvalidSessionHandle)
		{
			while (argIndex < argCount)
			{
				uint32 friendIndex = atoi(pArgs->GetArg(argIndex++));
				if (friendIndex < friendCount)
				{
					/*SteamUserID**/ pFriendID = new /*SteamUserID*/(pSteamFriends->GetFriendByIndex(friendIndex, k_EFriendFlagImmediate));
					CryUserID id(pFriendID);
					CrySessionHandle sh = ((CCryMatchMaking*)pLobbyService->GetMatchMaking())->CreateGameSessionHandle(lsh, CryMatchMakingInvalidConnectionUID);
					NetLog("[STEAM]: Sending invite to [%s]...", pSteamFriends->GetFriendPersonaName(pFriendID->m_steamID));
					pLobbyService->GetFriends()->FriendsSendGameInvite(0, sh, &id, 1, NULL, NULL, NULL);
				}
				else
				{
					/*log*/("[STEAM]: Invalid friend index [%d] - ignoring... (see steam_show_friends)", friendIndex);
				}
			}
		}
		else
		{
			/*log*/("[STEAM]: No hosted nub session...");
		}
	}
	else
	{
		/*log*/("[STEAM]: Person to invite not defined.");
	}
}

#endif
//Steam Service Int
/*steam service int here*/
/*steam appid int here*/
/*steam accound id here*/
/*CSteamID int here*/
//

//Steam Stats upload
/*steam stats upload here*/
//
