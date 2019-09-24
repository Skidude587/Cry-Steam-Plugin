#pragma once

#include <CrySystem/ICryPlugin.h>

#include <CryGame/IGameFramework.h>

#include <CryEntitySystem/IEntityClass.h>
#include <CryNetwork/INetwork.h>
#include <CryExtension/ClassWeaver.h>

// Some public Macros 

// The Steamworks API's are modular, you can use some subsystems without using others
// When USE_GS_AUTH_API is defined you get the following Steam features:
// - Strong user authentication and authorization
// - Game server matchmaking
// - VAC cheat protection
// - Access to achievement/community API's
// - P2P networking capability

// Remove this define to disable using the native Steam authentication and matchmaking system
// You can use this as a sample of how to integrate your game without replacing an existing matchmaking system
// When you un-define USE_GS_AUTH_API you get:
// - Access to achievement/community API's
// - P2P networking capability
// You CANNOT use:
// - VAC cheat protection
// - Game server matchmaking
// as these function depend on using Steam authentication
#define USE_GS_AUTH_API 
// UDP port for the game server to do authentication on (ie, talk to Steam on)
#define GAME_AUTH_PORT 8766
// UDP port for the game server to listen on
#define GAME_SERVER_PORT 27015
// UDP port for the master server updater to listen on
#define GAME_MASTER_SERVER_UPDATER_PORT 27016
// How long to wait for a client to send an update before we drop its connection server side
#define SERVER_TIMEOUT_MILLISECONDS 5000
// Maximum packet size in bytes
#define MAX_SPACEWAR_PACKET_SIZE 1024*512
// Time to timeout a connection attempt in
#define MILLISECONDS_CONNECTION_TIMEOUT 30000
// How many times a second does the server send world updates to clients
#define SERVER_UPDATE_SEND_RATE 60
// How many times a second do we send our updated client state to the server
#define CLIENT_UPDATE_SEND_RATE 30




class CPlugin 
	: public Cry::IEnginePlugin
	, public ISystemEventListener
	, public INetworkedClientListener

{
public:
	CRYINTERFACE_SIMPLE(Cry::IEnginePlugin)
	CRYGENERATE_SINGLETONCLASS_GUID(CPlugin, "MyPlugin", "2711a23d-3848-4cdd-a95b-e9d88ffa23b0"_cry_guid)


	virtual ~CPlugin();
	
	// Cry::IEnginePlugin
	virtual bool Initialize(SSystemGlobalEnvironment& env, const SSystemInitParams& initParams) override;
	// ~Cry::IEnginePlugin
	
	// ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// ~ISystemEventListener

	//------------------------
	// Added network listener might not be needed depending on games 
	//------------------------


	// INetworkedClientListener
	// Sent to the local client on disconnect
	virtual void OnLocalClientDisconnected(EDisconnectionCause cause, const char* description) override {}

	// Sent to the server when a new client has started connecting
	// Return false to disallow the connection
	virtual bool OnClientConnectionReceived(int channelId, bool bIsReset) override;

	// Sent to the server when a new client has finished connecting and is ready for gameplay
	// Return false to disallow the connection and kick the player
	virtual bool OnClientReadyForGameplay(int channelId, bool bIsReset) override;

	// Sent to the server when a client is disconnected
	virtual void OnClientDisconnected(int channelId, EDisconnectionCause cause, const char* description, bool bKeepClient) override;

	// Sent to the server when a client is timing out (no packets for X seconds)
	// Return true to allow disconnection, otherwise false to keep client.

	virtual bool OnClientTimingOut(int channelId, EDisconnectionCause cause, const char* description) override { return true; }
	// ~INetworkedClientListener


protected:
	// Map containing player components, key is the channel id received in OnClientConnectionReceived
	std::unordered_map<int, EntityId> m_players;


};