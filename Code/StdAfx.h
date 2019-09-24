// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#pragma once

#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule eCryM_EnginePlugin
#define GAME_API   DLL_EXPORT

#include <CryCore/Platform/platform.h>
#include <CrySystem/ISystem.h>
#include <Cry3DEngine/I3DEngine.h>
#include <CryNetwork/ISerialize.h>

#include "public/steam/steam_api.h"
#include "public/steam/isteamuserstats.h"
#include "public/steam/isteamremotestorage.h"
#include "public/steam/isteammatchmaking.h"
#include "public/steam/steam_gameserver.h"