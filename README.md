#NEED TO ADD PROPER README
##What it does
This plugin is a replacement for the currently broken CryGamePlatformSteam. It calls up the Steam API and initiates it.
It also sets up online lobbies hosted via your Steam AppId.
##How To
Copy code to the proper project folders.
Add CrySteamPlugin in your required plugins section in your .cryproject file.
Add in your Steam AppId to "sys_steamAppId" in your .cryproject file.
In LobbyCVars.cpp change "net_usesteamasonlinelobby" to 1.
(IF USING GAME SDK) Change the AppId in gamebrowser.cpp to your Steam AppId.
