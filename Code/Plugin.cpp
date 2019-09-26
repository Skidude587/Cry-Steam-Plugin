//
// -------------------------------------------------------------------------
//  File name:   Plugin.cpp
//  Created:     23/09/2019 by Jonathan Green, Von Bismarck
//  Description: Main File for the Plugin
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"

#include "Plugin.h"

#include <steam_api.h>

#include "lobby/SteamLobby.h"
#include "Components/SteamOnlineSystemComponent.h"

#include <PlatformTypes.h>
#include <PlatformIdentifier.h>
#include <IPlatformService.h>
#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>
// Included only once per DLL module.
#include <CryCore/Platform/platform_impl.inl>


CPlugin::~CPlugin()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RemoveListener(this);

	if (gEnv->pSchematyc)
	{
		gEnv->pSchematyc->GetEnvRegistry().DeregisterPackage(CPlugin::GetCID());
	}
}


bool CPlugin::Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams)
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this,"CPlugin");

	return true;
}

void CPlugin::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_REGISTER_SCHEMATYC_ENV:
	{
		// Register all components that belong to this plug-in
		auto staticAutoRegisterLambda = [](Schematyc::IEnvRegistrar& registrar)
		{
			// Call all static callback registered with the CRY_STATIC_AUTO_REGISTER_WITH_PARAM
			Detail::CStaticAutoRegistrar<Schematyc::IEnvRegistrar&>::InvokeStaticCallbacks(registrar);
		};

		if (gEnv->pSchematyc)
		{
			gEnv->pSchematyc->GetEnvRegistry().RegisterPackage(
				stl::make_unique<Schematyc::CEnvPackage>(
					CPlugin::GetCID(),
					"EntityComponents",
					"Crytek GmbH",
					"Components",
					staticAutoRegisterLambda
					)
			);
		}
	}
	break;

	case ESYSTEM_EVENT_GAME_POST_INIT:
	{

		// Listen for client connection events, in order to create the local player
		gEnv->pGameFramework->AddNetworkedClientListener(*this);

#if USING_STEAM == 1
		
		/* Using this process allows Steam to start as needed. All successful :D - Von */
		if (!SteamAPI_RestartAppIfNecessary(480))
		{
			// if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
			// local Steam client and also launches this game again.

			// Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
			// removed steam_appid.txt from the game depot.
			CRY_ASSERT("STEAM NEEDS TO BE RUNNING! ");
			return;
		}

		if (!SteamAPI_Init())
		{
			CRY_ASSERT("SteamAPI_Init() failed\n");
			CRY_ASSERT("Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n");
			return;
		}

		// Ensure that the user has logged into Steam. This will always return true if the game is launched
		// from Steam, but if Steam is at the login prompt when you run your game from the debugger, it
		// will return false.
		if (!SteamUser()->BLoggedOn())
		{
			CRY_ASSERT("Steam user is not logged in\n");
			CRY_ASSERT("Fatal Error", "Steam user must be logged in to play this game (SteamUser()->BLoggedOn() returned false).\n");
			return;
		}

		if (!SteamInput()->Init())
		{
			CRY_ASSERT("SteamInput()->Init failed.\n");
			CRY_ASSERT("Fatal Error", "SteamInput()->Init failed.\n");
			return;
		}
		
#pragma comment(lib, "steam_api64.lib")
		
		/* Might need to move this function somewhere else */
		m_pCSteamLobby->GetLocalClient();
		// Shutdown the SteamAPI - needed 
		SteamAPI_Shutdown();
#endif // USING_STEAM


		/* Change back to release after testing... */
#if (PROFILE)
		//Steam Friend service
		static void SteamShowFriends_DevelopmentOnly(IConsoleCmdArgs* pArgs);
		{
			ISteamFriends* pSteamFriends = SteamFriends();
			if (!pSteamFriends)
			{
				/*Warning*/CRY_ASSERT(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Steam friends service not available or working, check log");
				return;
			}
			uint32 friendCount = pSteamFriends->GetFriendCount(k_EFriendFlagImmediate);
			/*log*/CryLog("[STEAM]: Friends list (%d friends):", friendCount);
			for (uint32 index = 0; index < friendCount; ++index)
			{
				CSteamID friendID = pSteamFriends->GetFriendByIndex(index, k_EFriendFlagImmediate);
				/*Need to creating logging*/
			}
		}
		//

		//Steam overlay
		static void SteamShowOverlay_DevelopmentOnly(IConsoleCmdArgs* pArgs);
		{
			ISteamUtils* pSteamUtils = SteamUtils();
			ISteamFriends* pSteamFriends = SteamFriends();
			if (!pSteamUtils || !pSteamFriends)
			{
				/*Warning*/CRY_ASSERT(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Overlay cannot be shown; Steam is not initialized");
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
					/*Log*/CryLog("Illegal overlay index specified [%d]:", requestedOverlay);
					for (uint32 index = 0; index < numOverlays; ++index)
					{
						/*Log*/CryLog("  [%d] %s", index, overlay[index]);
					}
					return;
				}
			}

			if (pSteamUtils->IsOverlayEnabled() == false)
			{
				/*Log*/CryLog("[STEAM]: Attempting to show [%s] overlay", overlay[requestedOverlay]);
				pSteamFriends->ActivateGameOverlay(overlay[requestedOverlay]);
			}
			else
			{
				/*Log*/CryLog("[STEAM]: overlay already displayed");
			}
		}
	#endif


	}
	break;

	case ESYSTEM_EVENT_LEVEL_LOAD_END:
		// Clear the players from net, if didn't already. might need to remove this during testing... 
		m_players.clear();
	}
}

bool CPlugin::OnClientConnectionReceived(int channelId, bool bIsReset)
{
	return true;
}

bool CPlugin::OnClientReadyForGameplay(int channelId, bool bIsReset)
{
	return true;
}

void CPlugin::OnClientDisconnected(int channelId, EDisconnectionCause cause, const char * description, bool bKeepClient)
{
}

CRYREGISTER_SINGLETON_CLASS(CPlugin)