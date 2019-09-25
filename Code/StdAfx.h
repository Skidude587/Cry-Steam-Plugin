// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT
#define USING_STEAM 1

#include <CryCore/Platform/platform.h>
#include <CrySystem/ISystem.h>
#include <Cry3DEngine/I3DEngine.h>
#include <CryNetwork/ISerialize.h>



#ifdef STEAM_CEG
// Steam DRM header file
#include "cegclient.h"
#else
#define Steamworks_InitCEGLibrary() (true)
#define Steamworks_TermCEGLibrary()
#define Steamworks_TestSecret()
#define Steamworks_SelfCheck()
#endif