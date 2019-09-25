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
	if (features & eCLSO_Base)
	{
		//LOBBY_AUTO_LOCK;

		for (uint32 i = 0; i < MAX_LOBBY_TASKS; i++)
		{
			m_tasks[i].used = false;
		}
	}

	return eCLE_Success;
}


ECryLobbyError CLobbyService::Terminate(ECryLobbyServiceFeatures features, LobbyServiceCallback pCB)
{
	if (features & eCLSO_Base)
	{
		LOBBY_AUTO_LOCK;

		FROM_GAME_TO_LOBBY(&CLobbyService::FreeSocketNT, this);
	}

	return eCLE_Success;
}

ICryTCPServicePtr CLobbyService::GetTCPService(const char * pService)
{

}

ICryTCPServicePtr CLobbyService::GetTCPService(const char * pServer, uint16 port, const char * pUrlPrefix)
{
	return ICryTCPServicePtr();
}

// If User not allowed into game return to lobby
ECryLobbyError CLobbyService::GetUserPrivileges(uint32 user, CryLobbyTaskID * pTaskID, LobbyPrivilegeCallback pCB, void * pCBArg)
{
	ECryLobbyError error;
	CryLobbyServiceTaskID tid;

	LOBBY_AUTO_LOCK;

	error = StartTask(eT_GetUserPrivileges, user, false, &tid, pTaskID, (void*)pCB, pCBArg);

	if (error == eCLE_Success)
	{
		FROM_GAME_TO_LOBBY(&CLobbyService::StartTaskRunning, this, tid);
	}

	return error;
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
	listenPort = CLobbyCVars::Get().lobbyDefaultPort;
	connectPort = CLobbyCVars::Get().lobbyDefaultPort;
}

void CLobbyService::CancelTask(CryLobbyTaskID lTaskID)
{
	LOBBY_AUTO_LOCK;

	CryLogAlways("[Lobby]Try cancel task %u", lTaskID);

	if (lTaskID != CryLobbyInvalidTaskID)
	{
		for (uint32 i = 0; i < MAX_LOBBY_TASKS; i++)
		{
			STask* pTask = GetTask(i);

			CRY_ASSERT(pTask, "CCryLobby: Task base pointers not setup");

			if (pTask->used && (pTask->lTaskID == lTaskID))
			{
				CryLogAlways("[Lobby] Task %u canceled", lTaskID);
				pTask->pCB = NULL;
				break;
			}
		}
	}
}

void CLobbyService::CreateSocketNT(void)
{
	m_pLobby->InternalSocketCreate(m_service);
}

void CLobbyService::FreeSocketNT(void)
{
	m_pLobby->InternalSocketFree(m_service);
}


ECryLobbyError CLobbyService::StartTask(uint32 eTask, uint32 user, bool startRunning, CryLobbyServiceTaskID * pLSTaskID, CryLobbyTaskID * pLTaskID, void * pCb, void * pCbArg)
{
	CryLobbyTaskID lobbyTaskID = m_pLobby->CreateTask();

	if (lobbyTaskID != CryLobbyInvalidTaskID)
	{
		for (uint32 i = 0; i < MAX_LOBBY_TASKS; i++)
		{
			STask* pTask = GetTask(i);

			if (!pTask->used)
			{
				pTask->user = user;
				pTask->lTaskID = lobbyTaskID;
				pTask->error = eCLE_Success;
				pTask->startedTask = eTask;
				pTask->pCB = pCb;
				pTask->pCBArg = pCbArg;
				pTask->used = true;
				pTask->running = startRunning;

				if (pLSTaskID)
				{
					*pLSTaskID = i;
				}

				if (pLTaskID)
				{
					*pLTaskID = lobbyTaskID;
				}

				for (uint32 j = 0; j < MAX_LOBBY_TASK_DATAS; j++)
				{
					pTask->dataMem[j] = TMemInvalidHdl;
					pTask->dataNum[j] = 0;
				}

				return eCLE_Success;
			}
		}

		m_pLobby->ReleaseTask(lobbyTaskID);
	}

	return eCLE_TooManyTasks;
}

void CLobbyService::FreeTask(CryLobbyServiceTaskID lsTaskID)
{
	STask* pTask = GetTask(lsTaskID);

	m_pLobby->ReleaseTask(pTask->lTaskID);

	for (uint32 i = 0; i < MAX_LOBBY_TASK_DATAS; i++)
	{
		if (pTask->dataMem[i] != TMemInvalidHdl)
		{
			m_pLobby->MemFree(pTask->dataMem[i]);
			pTask->dataMem[i] = TMemInvalidHdl;
		}
	}

	pTask->used = false;
}


void CLobbyService::UpdateTaskError(CryLobbyServiceTaskID lsTaskID, ECryLobbyError error)
{
	STask* pTask = GetTask(lsTaskID);

	if (pTask->error == eCLE_Success)
	{
		pTask->error = error;
	}
}

void CLobbyService::StartTaskRunning(CryLobbyServiceTaskID lsTaskID)
{
	LOBBY_AUTO_LOCK;

	STask* pTask = GetTask(lsTaskID);

	if (pTask->used)
	{
		pTask->running = true;

		switch (pTask->startedTask)
		{
		case eT_GetUserPrivileges:
			StopTaskRunning(lsTaskID);
			break;
		}
	}
}

void CLobbyService::StopTaskRunning(CryLobbyServiceTaskID lsTaskID)
{
	STask* pTask = GetTask(lsTaskID);

	if (pTask->used)
	{
		pTask->running = false;

		TO_GAME_FROM_LOBBY(&CLobbyService::EndTask, this, lsTaskID);
	}
}
void CLobbyService::EndTask(CryLobbyServiceTaskID lsTaskID)
{
	LOBBY_AUTO_LOCK;

	STask* pTask = GetTask(lsTaskID);

	if (pTask->used)
	{
		if (pTask->pCB)
		{
			switch (pTask->startedTask)
			{
			case eT_GetUserPrivileges:
				if (!m_pLobby->GetInternalSocket(m_service))
				{
					UpdateTaskError(lsTaskID, eCLE_InternalError);
				}

				((CryLobbyPrivilegeCallback)pTask->pCB)(pTask->lTaskID, pTask->error, 0, pTask->pCBArg);
				break;
			}
		}

		if (pTask->error != eCLE_Success)
		{
			NetLog("[Lobby] Lobby Service EndTask %u Result %d", pTask->startedTask, pTask->error);
		}

		FreeTask(lsTaskID);
	}
}

ECryLobbyError CLobbyService::CreateTaskParamMem(CryLobbyServiceTaskID lsTaskID, uint32 param, const void * pParamData, size_t paramDataSize)
{
	STask* pTask = &m_tasks[lsTaskID];

	CRY_ASSERT(pTask, "CCryLobbyService: Task base pointers not setup");

	if (paramDataSize > 0)
	{
		pTask->dataMem[param] = m_pLobby->MemAlloc(paramDataSize);
		void* p = m_pLobby->MemGetPtr(pTask->dataMem[param]);

		if (p)
		{
			if (pParamData)
			{
				memcpy(p, pParamData, paramDataSize);
			}
		}
		else
		{
			return eCLE_OutOfMemory;
		}
	}

	return eCLE_Success;
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
