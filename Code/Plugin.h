#pragma once

#include <CrySystem/ICryPlugin.h>

#include <CryGame/IGameFramework.h>

#include <CryEntitySystem/IEntityClass.h>

//steam api
#include <steam/steam_api.h>

//normal steam stuff
#include <steam/isteamapplist.h>
#include <steam/isteamapps.h>
#include <steam/isteamappticket.h>
#include <steam/isteamclient.h>
#include <steam/isteamcontroller.h>
#include <steam/isteamfriends.h>
#include <steam/isteamgamecoordinator.h>
#include <steam/isteamgameserver.h>
#include <steam/isteamgameserverstats.h>
#include <steam/isteamhtmlsurface.h>
#include <steam/isteamhttp.h>
#include <steam/isteaminput.h>
#include <steam/isteaminventory.h>
#include <steam/isteammasterserverupdater.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteammusic.h>
#include <steam/isteammusicremote.h>
#include <steam/isteamnetworking.h>
#include <steam/isteamnetworkingsockets.h>
#include <steam/isteamnetworkingutils.h>
#include <steam/isteamparentalsettings.h>
#include <steam/isteamps3overlayrenderer.h>
#include <steam/isteamremoteplay.h>
#include <steam/isteamremotestorage.h>
#include <steam/isteamscreenshots.h>
#include <steam/isteamugc.h>
#include <steam/isteamuser.h>
#include <steam/isteamutils.h>
#include <steam/isteamvideo.h>
#include <steam/matchmakingtypes.h>
#include <steam/steam_api_common.h>
#include <steam/steam_api_internal.h>
#include <steam/steam_gameserver.h>
#include <steam/steamclientpublic.h>
#include <steam/steamdatagram_tickets.h>
#include <steam/steamencryptedappticket.h>
#include <steam/steamhttpenums.h>
#include <steam/steamnetworkingtypes.h>
#include <steam/steamps3params.h>
#include <steam/steamtypes.h>
#include <steam/steamuniverse.h>


class CPlugin 
	: public Cry::IEnginePlugin
	, public ISystemEventListener
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
};