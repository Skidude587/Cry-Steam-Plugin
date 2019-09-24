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
#include <../../CryPlugins/CryGamePlatform/Interface/IGamePlatform.h>
#include <../../CryPlugins/CryLobby/Module/CryLobby.h>
#include <../../CryPlugins/CryLobby/Interface/CryLobby/ICryLobbyPrivate.h>
#include "friends/SteamFriends.h"
#include "lobby/NetLog.h"

#if (RELEASE)
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
					NetLog("[STEAM]: Invalid friend index [%d] - ignoring... (see steam_show_friends)", friendIndex);
				}
			}
		}
		else
		{
			NetLog("[STEAM]: No hosted nub session...");
		}
	}
	else
	{
		NetLog("[STEAM]: Person to invite not defined.");
	}
}

#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*SteamLobbyService*/::/*SteamLobbyService*/(CCryLobby* pLobby, ECryLobbyService service)
	: /*SteamLobbyService*/(pLobby, service)

/*SteamLobbyService*/::/*~SteamLobbyService*/(void)
{
#if (RELEASE)
	gEnv->pConsole->UnregisterVariable("steam_invite_to_game", true);
	gEnv->pConsole->UnregisterVariable("steam_show_friends", true);
	gEnv->pConsole->UnregisterVariable("steam_show_overlay", true);
#endif // !defined(RELEASE)
}

ECryLobbyError /*SteamLobbyService*/::Initialise(ECryLobbyServiceFeatures features, CryLobbyServiceCallback pCB)
{
	ECryLobbyError ret = eCLE_Success;

	if (SteamUser() == NULL)
	{
		if ((pPlugin == nullptr) ||
			(pPlugin->GetMainService() == nullptr)
		{
			ret = eCLE_NotInitialised;
		}

		if (ret != eCLE_Success)
		{
			NetLog("[STEAM]: SteamAPI_Init() failed");
			return ret;
		}
		NetLog("[STEAM]: SteamAPI_Init() succeeded");
	}

	ret = CCryLobbyService::Initialise(features, pCB);

#if USE_CRY_MATCHMAKING
	if ((ret == eCLE_Success) && (m_pMatchmaking == NULL) && (features & eCLSO_Matchmaking))
	{
		m_pMatchmaking = new CCrySteamMatchMaking(m_pLobby, this, m_service);

		if (m_pMatchmaking != NULL)
		{
			ret = m_pMatchmaking->Initialise();
		}
		else
		{
			return eCLE_OutOfMemory;
		}
	}
#endif // USE_CRY_MATCHMAKING

//Steam Service Int
/*steam service int here*/
/*steam appid int here*/
/*steam accound id here*/
/*CSteamID int here*/
//

//Steam Stats upload
/*steam stats upload here*/
//
