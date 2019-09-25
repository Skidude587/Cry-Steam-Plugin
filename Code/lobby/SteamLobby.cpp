//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.cpp
//  Created:     23/09/2019 by Jonathan Green, 
//  Description: Creating the Multiplayer Steam Lobby
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SteamLobby.h"
#include "friends/SteamFriends.h"
#include "server/SteamServer.h"
#include <steam_api.h>

CLobbyService::~CLobbyService()
{
}

ECryLobbyError CLobbyService::Initialise(ECryLobbyServiceFeatures features, LobbyServiceCallback pCB)
{
	return ECryLobbyError();
}

ECryLobbyError CLobbyService::Terminate(ECryLobbyServiceFeatures features, LobbyServiceCallback pCB)
{
	return ECryLobbyError();
}

ICryTCPServicePtr CLobbyService::GetTCPService(const char * pService)
{
	return ICryTCPServicePtr();
}

ICryTCPServicePtr CLobbyService::GetTCPService(const char * pServer, uint16 port, const char * pUrlPrefix)
{
	return ICryTCPServicePtr();
}

ECryLobbyError CLobbyService::GetUserPrivileges(uint32 user, CryLobbyTaskID * pTaskID, LobbyPrivilegeCallback pCB, void * pCBArg)
{
	return ECryLobbyError();
}

void CLobbyService::MakeAddrPCCompatible(TNetAddress & addr)
{
}

bool CLobbyService::GetFlag(ECryLobbyServiceFlag flag)
{
	return false;
}

void CLobbyService::GetSocketPorts(uint16 & connectPort, uint16 & listenPort)
{
}

void CLobbyService::CancelTask(CryLobbyTaskID lTaskID)
{
}

void CLobbyService::CreateSocketNT(void)
{
}

void CLobbyService::FreeSocketNT(void)
{
}

ECryLobbyError CLobbyService::StartTask(uint32 eTask, uint32 user, bool startRunning, CryLobbyServiceTaskID * pLSTaskID, CryLobbyTaskID * pLTaskID, void * pCb, void * pCbArg)
{
	return ECryLobbyError();
}

void CLobbyService::FreeTask(CryLobbyServiceTaskID lsTaskID)
{
}

void CLobbyService::UpdateTaskError(CryLobbyServiceTaskID lsTaskID, ECryLobbyError error)
{
}

void CLobbyService::StartTaskRunning(CryLobbyServiceTaskID lsTaskID)
{
}

void CLobbyService::StopTaskRunning(CryLobbyServiceTaskID lsTaskID)
{
}

void CLobbyService::EndTask(CryLobbyServiceTaskID lsTaskID)
{
}

ECryLobbyError CLobbyService::CreateTaskParamMem(CryLobbyServiceTaskID lsTaskID, uint32 param, const void * pParamData, size_t paramDataSize)
{
	return ECryLobbyError();
}

CMementoMemoryManager::CMementoMemoryManager(const string & name)
{
}

CMementoMemoryManager::~CMementoMemoryManager()
{
}

void * CMementoMemoryManager::AllocPtr(size_t sz, void * callerOverride)
{
	return nullptr;
}

void CMementoMemoryManager::FreePtr(void * p, size_t sz)
{
}


void CMementoMemoryManager::ResizeHdl(Hdl hdl, size_t sz)
{
}

void CMementoMemoryManager::FreeHdl(Hdl hdl)
{
}

void CMementoMemoryManager::AddHdlToSizer(Hdl hdl, ICrySizer * pSizer)
{
}

void CMementoMemoryManager::GetMemoryStatistics(ICrySizer * pSizer, bool countingThis)
{
}

void CMementoMemoryManager::DebugDraw()
{
}

void CMementoMemoryManager::Tick()
{
}

CMementoMemoryManager::CMementoMemoryManagerAllocator::CMementoMemoryManagerAllocator()
{
}

CMementoMemoryManager::CMementoMemoryManagerAllocator::~CMementoMemoryManagerAllocator()
{
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::AddCMementoMemoryManager()
{
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::RemoveCMementoMemoryManager()
{
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::Tick()
{
}


void CMementoMemoryManager::CMementoMemoryManagerAllocator::FreeHdl(Hdl hdl)
{
}

void * CMementoMemoryManager::CMementoMemoryManagerAllocator::AllocPtr(size_t sz)
{
	return nullptr;
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::FreePtr(void * p, size_t sz)
{
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::ResizeHdl(Hdl hdl, size_t sz)
{
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::InitHandleData(SHandleData & hd, size_t sz)
{
}

void CMementoMemoryManager::CMementoMemoryManagerAllocator::DebugDraw(int x, int & y, size_t & totalAllocated)
{
}

ECryLobbyError CSteamLobbySystem::GetSystemTime(uint32 user, SSystemTime * pSystemTime)
{
	return ECryLobbyError();
}

void CSteamLobbySystem::OnPacket(const TNetAddress & addr, CCryLobbyPacket * pPacket)
{
}

void CSteamLobbySystem::OnError(const TNetAddress & addr, ESocketError error, LobbySendID sendID)
{
}

void CSteamLobbySystem::OnSendComplete(const TNetAddress & addr, LobbySendID sendID)
{
}

void CSteamLobbySystem::GetSocketPorts(uint16 & connectPort, uint16 & listenPort)
{
}

void CSteamLobbySystem::InviteAccepted(uint32 user, CrySessionID id)
{
}

CSteamLobbySystem::STEAM_CALLBACK(CSteamLobbySystem, OnPersonaStateChange, PersonaStateChange_t, m_CallbackPersonaStateChange)
{
}

CSteamLobbySystem::STEAM_CALLBACK(CSteamLobbySystem, OnLobbyDataUpdate, LobbyDataUpdate_t, m_CallbackLobbyDataUpdate)
{
}

CSteamLobbySystem::STEAM_CALLBACK(CSteamLobbySystem, OnLobbyChatUpdate, LobbyChatUpdate_t, m_CallbackChatDataUpdate)
{
}
