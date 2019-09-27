//
// -------------------------------------------------------------------------
//  File name:   SteamOnlineLobbySystemComponent.h
//  Created:     23/09/2019 by Von Bismarck, Jonathan Green
//  Description: This Class file is for calling Steam API for our lobby system
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"

#include <steam_api.h>

#include <array>
#include <numeric>

#include <CryEntitySystem/IEntityComponent.h>
#include <CrySchematyc/Utils/EnumFlags.h>
#include <CrySchematyc/ResourceTypes.h>
#include <CrySchematyc/Reflection/TypeDesc.h>
#include <CrySchematyc/MathTypes.h>
#include <CrySchematyc/Env/IEnvRegistrar.h>
#include <CrySchematyc/CoreAPI.h>

class CSteamLobbySystemComponent : public IEntityComponent
{


public:
	CSteamLobbySystemComponent() = default;
	virtual ~CSteamLobbySystemComponent() {}

	// IEntityComponent
	void Initialize() override;
	virtual void ProcessEvent(const SEntityEvent& event) override;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	// ~IEntityComponent 

	

	static void ReflectType(Schematyc::CTypeDesc<CSteamLobbySystemComponent>& desc)
	{
		desc.SetGUID(CSteamLobbySystemComponent::IID());
		desc.SetEditorCategory("Steam");
		desc.SetLabel("Steam");
		desc.SetDescription("This component allows you to add Steam and configure it");
		desc.SetIcon("icons:ObjectTypes/light.ico");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

		desc.AddMember(&CSteamLobbySystemComponent::UseSteam, 'ustm', "UseSteam", "Use Steam", "Use Steam", false);
		desc.AddMember(&CSteamLobbySystemComponent::m_Steam_appId, 'text', "SteamID", "Steam ID", "Your App ID from Steam", "");

		desc.AddMember(&CSteamLobbySystemComponent::m_LobbyName, 'dfln', "DefaultLobbyName", "Default Lobby Name", "Default Lobby Name", "");
		desc.AddMember(&CSteamLobbySystemComponent::SteamServer, 'usts', "SteamServer", "Use Steam Server", "Use Steam Server", false);
		desc.AddMember(&CSteamLobbySystemComponent::serverport, 'svrp', "ServerPort", "Server Port", "Server Port", 0);
		desc.AddMember(&CSteamLobbySystemComponent::serverIP, 'svri', "ServerIP", "Server IP Address", "Server IP Address", 0);
		desc.AddMember(&CSteamLobbySystemComponent::lobbySize, 'lbbz', "LobbySize", "Lobby Size", "Lobby Size", 0);

		desc.AddMember(&CSteamLobbySystemComponent::SteamFriends, 'ustf', "SteamFriends", "Use Friends", "Use Friends", false);

	}

	static CryGUID& IID()
	{
		static CryGUID id = "{C70CABB7-4BD1-471C-B055-3363C431D9D0}"_cry_guid;
		return id;
	}

	/* Using Steam or Not */
	bool UsingSteam() { return UseSteam = USING_STEAM; }

	/* Using Steam Friends or not */
	bool UseSteamFriends() { return SteamFriends = USING_STEAM_FRIENDS; }

	/* Using Steam Server allow to use steam server instead of Cryengine server */
	bool UseSteamServer() { return SteamServer = USING_STEAM_SERVER; }

	/* Get lobby Name from User input */
	const char* GetLobbyName() { return m_LobbyName.c_str(); }

	/* Allow to change lobby size - Might need to cap it */
	int32 lobbySizeMax() { return lobbyDefaultSize = lobbySize; }

	/* Get SteamID */
	const char* GetSteamAppID() const { return m_Steam_appId.c_str(); }



protected:
	Schematyc::CSharedString m_Steam_appId;
	Schematyc::CSharedString m_LobbyName;

	int32 lobbyDefaultSize = 0;
	int32 lobbySize = 0;
	/* These valus might need changing */
	int32 serverport = 0;
	int32 serverIP = 0;

	/* More custom opitions - needs a return = operator too tired to do...  */
	
	

	/* Default Component   */
	bool UseSteam = false;
	bool SteamFriends = false;
	bool SteamServer = false;
 
};

