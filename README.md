# CrySteamPlugin
A plugin to replace CryGamePlatformSteam.
## What it does
This plugin is a replacement for the currently broken CryGamePlatformSteam. It calls up the Steam API and initiates it.

It also sets up online lobbies hosted via your Steam AppId.
## How To
Copy code to the proper folders.

Add CrySteamPlugin in your required plugins section in your .cryproject file.
Eg:```"plugins": [
               { 
                "type": "EPluginType::Native",
                "path": "CrySteamPlugin"
                }
            ]```

Add in your Steam AppId to ```"sys_steamAppId"``` in your .cryproject file.

In *LobbyCVars.cpp* change ```"net_useSteamAsOnlineLobby"``` to 1.

*(IF USING GAME SDK)*
Change the AppId in *GameBrowser.cpp* to your Steam AppId.

*(IF IN RELEASE NOT LAUCNING VIA STEAM)*
Make sure you have a *steam_appid.txt* containing ONLY your appid in your bin folder.




## .cryproject Example
```
{
    "version": 4,
    "type": "CRYENGINE Project",
    "info": {
        "name": "GameSDK",
        "guid": "981c2e0b-5ce6-ce71-f0be-eab93bbd43f6"
    },
    "content": {
        "assets": [
            "GameSDK"
        ],
        "code": [
            ""
        ],
        "libs": [
            {
                "name": "GameSDK",
                "shared": {
                    "any": "CryGameSDK",
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
                "path": "CrySteamPlugin"
            }
        ]
    },
    "console_variables": {
        "sys_ime": "1",
        "sys_splashscreen": "Libs/UI/textures/startscreen.tif",
        "sys_target_platforms": "pc,ps4,xboxone,linux"
        ```diff +"sys_steamAppId": "1160200"
    },
    "console_commands": {}
}
```
