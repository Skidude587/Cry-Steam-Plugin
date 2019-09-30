# CrySteamPlugin
A plugin to replace CryGamePlatformSteam.
## What it does
This plugin is a replacement for the currently broken CryGamePlatformSteam. It calls up the Steam API and initiates it.

It also sets up online lobbies hosted via your Steam AppId.

WARNING: This plugin does not work with GameSDK.
## How To
If you aren't building it yourself you can copy the "Plugin.dll" to the bin folder of your project. And adjust your .cryproject as seen below.

Copy code to the proper folders.

Add "Plugin.dll" in your required plugins section in your .cryproject file.
Eg:```"plugins": [
               { 
                "type": "EPluginType::Native",
                "path": "bin\win_x64\Plugin.dll"
                }
            ]```

Add in your Steam AppId to ```"sys_steamAppId"``` in your .cryproject file.

In *LobbyCVars.cpp* change ```"net_useSteamAsOnlineLobby"``` to 1.

*(IF IN RELEASE NOT LAUCNING VIA STEAM)*
Make sure you have a *steam_appid.txt* containing ONLY your appid in your bin folder.




## .cryproject Example
```
{
    "version": 4,
    "type": "CRYENGINE Project",
    "info": {
        "name": "Game",
        "guid": "981c2e0b-5ce6-ce71-f0be-eab93bbd43f6"
    },
    "content": {
        "assets": [
            "Game"
        ],
        "code": [
            ""
        ],
        "libs": [
            {
                "name": "Game",
                "shared": {
                    "any": "Game",
                    "win_x64": "",
                    "win_x86": ""
                }
            }
        ]
    },
    "require": {
        "engine": "engine-5.6",
        "plugins": [
            {
                "type": "EPluginType::Native",
                "path": "CryDefaultEntities"
            },
            {
                "type": "EPluginType::Native",
                "path": "CrySensorSystem"
            },
            {
                "type": "EPluginType::Native",
                "path": "CryPerceptionSystem"
            },
            {
                "type": "EPluginType::Native",
                "path": "CryUQS_Core"
            },
            {
                "type": "EPluginType::Native",
                "path": "CryGamePlatform",
                "platforms": [ "Windows" ]
            },
            {
                "type": "EPluginType::Native",
                "path": "CryLobby"
            },
            {
                "type": "EPluginType::Native",
                "path": "bin\win_x64\Plugin.dll"
            }
        ]
    },
    "console_variables": {
        "sys_ime": "1",
        "sys_splashscreen": "Libs/UI/textures/startscreen.tif",
        "sys_target_platforms": "pc,ps4,xboxone,linux"
        "sys_steamAppId": "480",
	"net_useSteamAsOnlineLobby": "1"
    },
    "console_commands": {}
}
```
## LobbyCVars.cpp Example
```
#if USE_STEAM
	REGISTER_CVAR2_DEV_ONLY("net_lobby_steam_online_port", &lobbySteamOnlinePort, STEAM_GAME_DEFAULT_PORT, VF_NULL,);
	REGISTER_CVAR2("net_useSteamAsOnlineLobby", &useSteamAsOnlineLobby, 1, VF_REQUIRE_APP_RESTART,);
	REGISTER_CVAR2("net_steam_resetAchievements", &resetSteamAchievementsOnBoot, 0, VF_REQUIRE_APP_RESTART, );
#endif 
```

