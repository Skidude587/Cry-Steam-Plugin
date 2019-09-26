cmake_minimum_required (VERSION 3.14)
set(CRYENGINE_DIR "C:/Users/jonat/Desktop/Cryengine/Engines/Crytek/CRYENGINE_5.6" CACHE STRING "CRYENGINE root directory.")
set(TOOLS_CMAKE_DIR "${CRYENGINE_DIR}/Tools/CMake")

set(PROJECT_BUILD 1)
set(PROJECT_DIR "${CMAKE_SOURCE_DIR}/../")

include("${TOOLS_CMAKE_DIR}/InitialSetup.cmake")
include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")

add_subdirectory("${CRYENGINE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/CRYENGINE")

include("${TOOLS_CMAKE_DIR}/Configure.cmake")
start_sources()

sources_platform(ALL)
add_sources("Code_uber.cpp"
    PROJECTS Plugin
    SOURCE_GROUP "Root"
		"Plugin.cpp"
		"StdAfx.cpp"
		"Plugin.h"
		"StdAfx.h"
)
add_sources("Components_uber.cpp"
    PROJECTS Plugin
    SOURCE_GROUP "Components"
		"Components/SteamOnlineSystemComponent.cpp"
		"Components/SteamOnlineSystemComponent.h"
)
add_sources("FlowGraph_uber.cpp"
    PROJECTS Plugin
    SOURCE_GROUP "FlowGraph"
		"FlowGraph/SteamOnlineFlowGraph.cpp"
		"FlowGraph/SteamOnlineFlowGraph.h"
)
add_sources("friends_uber.cpp"
    PROJECTS Plugin
    SOURCE_GROUP "friends"
		"friends/SteamFriends.cpp"
		"friends/SteamFriends.h"
)
add_sources("lobby_uber.cpp"
    PROJECTS Plugin
    SOURCE_GROUP "lobby"
		"lobby/SteamLobby.cpp"
		"lobby/SteamLobby.h"
)
add_sources("server_uber.cpp"
    PROJECTS Plugin
    SOURCE_GROUP "server"
		"server/SteamServer.cpp"
		"server/NetLog.h"
		"server/SteamServer.h"
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/CVarOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "CVarOverrides.h"
    )
endif()

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/EngineDefineOverrides.h")
    add_sources("NoUberFile"
        PROJECTS Game
        SOURCE_GROUP "Root"
            "EngineDefineOverrides.h"
    )
endif()
end_sources()

CryGameModule(Plugin FORCE_SHARED PCH "StdAfx.cpp" SOLUTION_FOLDER "Project")

target_include_directories(${THIS_PROJECT}
PRIVATE
    "${CRYENGINE_DIR}/Code/CryEngine/CryCommon"
    "${CRYENGINE_DIR}/Code/CryEngine/CryAction"
    "${CRYENGINE_DIR}/Code/CryEngine/CrySchematyc/Core/Interface"
    "${CRYENGINE_DIR}/Code/CryPlugins/CryDefaultEntities/Module"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "[Cc]lang")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-variable
        -Wno-reorder
        -Wno-unknown-pragmas
        -Wno-parentheses
        -Wno-switch
        -Wno-format
        -Wno-dynamic-class-memaccess
        -Wno-unused-private-field
        -Wno-unused-value
        -Wno-invalid-offsetof
        -Wno-multichar
        -Wno-char-subscripts
        -Wno-null-conversion
        -Wno-empty-body
        -Wno-unused-lambda-capture
        -Wno-unused-function
        -Wno-tautological-constant-out-of-range-compare
    )
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${THIS_PROJECT} PRIVATE
        -Wno-unused-function
        -Wno-unused-value
        -Wno-unused-variable
        -Wno-aligned-new
        -Wno-int-in-bool-context
        -Wno-invalid-offsetof
        -Wno-comment
        -Wno-sign-compare
        -Wno-unused-but-set-variable
        -Wno-maybe-uninitialized
        -Wno-misleading-indentation
        -Wno-unused-result
        -Wno-unknown-pragmas
        -Wno-multichar
        -Wno-strict-aliasing
        -Wno-char-subscripts
        -Wno-conversion-null
        -Wno-reorder
        -Wno-parentheses
        -Wno-format
        -Wno-switch
    )
endif()


if(OPTION_ENGINE)
    if(NOT EXISTS "${CRYENGINE_DIR}/Code/Sandbox/EditorQt")
        add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
        set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
        if (WIN32)
            set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}Plugin.cryproject\"")
        endif()
    endif()
else()
    add_library(GameLauncher STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameLauncher PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameLauncher "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}Plugin.cryproject\"")
    endif()

    add_library(Editor STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(Editor "${CRYENGINE_DIR}/bin/win_x64/Sandbox.exe" "-project \"${PROJECT_DIR}Plugin.cryproject\"")
    endif()

    add_library(GameServer STATIC "${CRYENGINE_DIR}/Code/CryEngine/CryCommon/CryCore/Platform/platform.h")
    set_target_properties(GameServer PROPERTIES LINKER_LANGUAGE CXX)
    if (WIN32)
        set_visual_studio_debugger_command(GameServer "${CRYENGINE_DIR}/bin/win_x64/Game_Server.exe" "-project \"${PROJECT_DIR}Plugin.cryproject\"")
    endif()
endif()

# Set StartUp project in Visual Studio
set_solution_startup_target(GameLauncher)

if (WIN32)
    set_visual_studio_debugger_command( ${THIS_PROJECT} "${CRYENGINE_DIR}/bin/win_x64/GameLauncher.exe" "-project \"${PROJECT_DIR}Plugin.cryproject\"" )
endif()

#BEGIN-CUSTOM
# Make any custom changes here, modifications outside of the block will be discarded on regeneration.
get_filename_component(BASE_PATH ${THIS_PROJECT}/../../../../ REALPATH)

add_library(steam_codegen STATIC "${CMAKE_SOURCE_DIR}/../sdk/public/steam/")
set_target_properties(steam_codegen PROPERTIES LINKER_LANGUAGE CXX)

if (CMAKE_CL_64 AND WIN32)
    if(_PROFILE)
        target_include_directories(${THIS_PROJECT} PRIVATE ../sdk/public/steam/lib/win32/include)
        target_include_directories(steam_codegen PRIVATE ../sdk/public/steam/lib/win32/include)
		file(GLOB_RECURSE LIB_LIST "../sdk/public/steam/lib/*.lib")
    else()
        target_include_directories(${THIS_PROJECT} PRIVATE ../sdk/public/steam/lib/win64/include)
        target_include_directories(steam_codegen PRIVATE ../sdk/public/steam/lib/win64/include)
		file(GLOB_RECURSE LIB_LIST "../sdk/public/steam/lib/*.lib")
	endif()
	target_compile_options(steam_codegen PRIVATE "/EHsc")
endif()

file(GLOB_RECURSE STEAM_CODEGEN_FILES_STD ${THIS_PROJECT}/public/steam/*.cpp ${THIS_PROJECT}/public/steam/*.h)

target_include_directories(${THIS_PROJECT} PRIVATE ${THIS_PROJECT}/public/steam/ )
target_include_directories(steam_codegen PRIVATE ${THIS_PROJECT}/public/steam/ )
target_sources(steam_codegen PRIVATE ${STEAM_CODEGEN_FILES_STD} )
target_link_libraries(steam_codegen PRIVATE ${LIB_LIST})

add_dependencies(${THIS_PROJECT} steam_codegen)


target_link_libraries(${THIS_PROJECT} PRIVATE steam_codegen ${LIB_LIST})

target_sources(${THIS_PROJECT} 
	PRIVATE
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamapplist.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamapps.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamappticket.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamclient.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamcontroller.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamfriends.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamgamecoordinator.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamgameserver.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamgameserverstats.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamhtmlsurface.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamhttp.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteaminput.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteaminventory.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteammasterserverupdater.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteammatchmaking.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteammusic.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteammusicremote.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamnetworking.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamnetworkingsockets.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamnetworkingutils.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamparentalsettings.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamps3overlayrenderer.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamremoteplay.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamremotestorage.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamscreenshots.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamugc.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamuser.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamuserstats.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamutils.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/isteamvideo.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/matchmakingtypes.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api_common.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api.json"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api_common.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api_flat.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api_internal.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_api_interop.cs"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steam_gameserver.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamclientpublic.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamdatagram_tickets.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamencryptedappticket.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamhttpenums.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamnetworkingtypes.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamps3params.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamtypes.h"
		"${CMAKE_SOURCE_DIR}/../sdk/public/steam/steamuniverse.h"
		
)
target_include_directories(${THIS_PROJECT}
	PRIVATE
	"${CMAKE_SOURCE_DIR}/../sdk/public/steam")
#END-CUSTOM