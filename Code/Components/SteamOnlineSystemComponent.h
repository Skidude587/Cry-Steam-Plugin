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
		desc.SetComponentFlags({ IEntityComponent::EFlags::Singleton });

		desc.AddMember(&CSteamLobbySystemComponent::UseSteam, 'ustm', "UseSteam", "Use Steam", "Use Steam", false);
		desc.AddMember(&CSteamLobbySystemComponent::SteamID, 'stmd', "SteamID", "Steam ID", "Steam ID", 0);

		desc.AddMember(&CSteamLobbySystemComponent::SteamServer, 'usts', "SteamServer", "Use Steam Server", "Use Steam Server", false);
		desc.AddMember(&CSteamLobbySystemComponent::serverport, 'svrp', "ServerPort", "Server Port", "Server Port", 0);
		desc.AddMember(&CSteamLobbySystemComponent::serverIP, 'svri', "ServerIP", "Server IP Address", "Server IP Address", 0);
		//desc.AddMember(&CSteamLobbySystemComponent::DefaultLobbyName, 'dfln', "DefaultLobbyName", "Default Lobby Name", "Default Lobby Name", "");
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
	//string GetLobbyName() { return LobbyName = DefaultLobbyName; }

	/* Allow to change lobby size - Might need to cap it */
	int32 lobbySizeMax() { return lobbyDefaultSize = lobbySize; }

	/* Get SteamID */
	int32 GetSteamGameID() { return GameID = SteamID; }



protected:

	int32 SteamID = 0;
	int32 GameID = 0;

	int32 lobbyDefaultSize = 0;
	int32 lobbySize = 0;
	/* These valus might need changing */
	int32 serverport = 0;
	int32 serverIP = 0;

	/* More custom opitions - needs a return = operator too tired to do...  */
	//string LobbyName = "";
	//string DefaultLobbyName = "";

	/* Default Component   */
	bool UseSteam = false;
	bool SteamFriends = false;
	bool SteamServer = false;
};

