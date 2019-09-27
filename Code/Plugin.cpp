#include "StdAfx.h"
#include "Plugin.h"
#include <steam_api.h>

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
	case ESYSTEM_EVENT_GAME_POST_INIT:
	{

		//ICVar* value = gEnv->pConsole->GetCVar("sys_steamAppId");
		//value->GetIVal();

		//CryLog("Youre steam_appid %d", value);
		/* Using this process allows Steam to start as needed. All successful :D - Von */
		if (!SteamAPI_RestartAppIfNecessary(1160200))
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
	}
	break;
	}
}

CRYREGISTER_SINGLETON_CLASS(CPlugin)