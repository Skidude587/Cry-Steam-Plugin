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

(IF USING GAME SDK) Change the AppId in *GameBrowser.cpp* to your Steam AppId.

(IF IN RELEASE NOT LAUCNING VIA STEAM) Make sure you have a *steam_appid.txt* containing ONLY your appid in your bin folder.
