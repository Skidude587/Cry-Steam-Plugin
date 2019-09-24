//
// -------------------------------------------------------------------------
//  File name:   Plugin.cpp
//  Created:     23/09/2019 by Crytek, Jonathan Green, Von Bismarck
//  Description: Main File for the Plugin
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
//All steam h files are in plugin.h
#include "Plugin.h"

#include <CrySchematyc/Env/IEnvRegistry.h>
#include <CrySchematyc/Env/EnvPackage.h>
#include <CrySchematyc/Utils/SharedString.h>
#include "lobby/SteamLobby.h"

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
		if (!SteamInput()->Init())
		{
			CRY_ASSERT("Steam needs to be running");

		}
#pragma comment(lib, "steam_api64.lib")
		//int LobbyCVars for steam lobbies 
		//int useSteamAsOnlineLobby;
		//int resetSteamAchievementsOnBoot;
		//int lobbySteamOnlinePort;
#endif // USING_STEAM


#if !defined(RELEASE)
		//Steam Friend service
		static void SteamShowFriends_DevelopmentOnly(IConsoleCmdArgs* pArgs);
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
		static void SteamShowOverlay_DevelopmentOnly(IConsoleCmdArgs* pArgs);
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
	#endif


	}
	break;

	case ESYSTEM_EVENT_LEVEL_LOAD_END:
		// Clear the players from net, if didn't already. might need to remove this during testing... 
		m_players.clear();
	}
}

CRYREGISTER_SINGLETON_CLASS(CPlugin)