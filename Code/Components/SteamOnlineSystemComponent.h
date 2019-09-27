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


class CSteamLobbySystemComponent : public IEntityComponent
{


	struct SSteam_appID
	{
		inline bool operator==(const SSteam_appID& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SSteam_appID& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		/* TODO Finish tomorrow mornging */
		string SteamID;
		
		
	};

	static void ReflectType(Schematyc::CTypeDesc<SSteam_appID>& desc)
	{
		desc.SetGUID("{FE63E77F-C8FA-45D4-B799-95C95F1EA6E3}"_cry_guid);
		desc.SetLabel("Steam App ID");
		desc.SetDescription("Steam App ID");
		desc.AddMember(&SSteam_appID::SteamID, 'stmd', "SteamID", "Steam ID", "Your App ID from Steam", "");
	}


	struct SSteam_Properties
	{
		inline bool operator==(const SSteam_Properties& rhs) const { return 0 == memcmp(this, &rhs, sizeof(rhs)); }
		inline bool operator!=(const SSteam_Properties& rhs) const { return 0 != memcmp(this, &rhs, sizeof(rhs)); }

		SSteam_appID sSteam_appID;
	};



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
		desc.AddMember(&SSteam_Properties::sSteam_appID, 'stmd', "SteamID", "Steam ID", "Steam ID", SSteam_appID());

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
	string GetSteamGameID() { return GameID = m_sSteamID->SteamID; }



protected:
	SSteam_appID* m_sSteamID;

	string GameID = 0;



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

