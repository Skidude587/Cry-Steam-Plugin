//
// -------------------------------------------------------------------------
//  File name:   SteamLobby.h
//  Created:     23/09/2019 by Von Bismarck, Jonathan Green
//  Description: This Class file is for calling Steam API for our lobby system
// -------------------------------------------------------------------------
//
////////////////////////////////////////////////////////////////////////////


#if _MSC_VER > 1000

#pragma once 
#endif
#include "StdAfx.h"
#include <steam_api.h>
#include <steam_gameserver.h>
#include <CryLobby/CommonICryLobby.h>
#include "server/NetLog.h"

//structs

class CLobbyCVars
{
public:
	CLobbyCVars();
	~CLobbyCVars();

	static CLobbyCVars& Get() { return *m_pThis; }

	float minMicrophonNotificationInterval;
	float serverPingNotificationInterval;
	int   showMatchMakingTasks;
	int   fullStatusOnClient;
	int   lobbyDefaultPort;
#if USE_STEAM
	int useSteamAsOnlineLobby;
	int resetSteamAchievementsOnBoot;
	int lobbySteamOnlinePort;
#endif // USE_STEAM
	static CLobbyCVars * m_pThis;
};
#define USE_CRY_MATCHMAKING          1
typedef void (*CryLobbyPrivilegeCallback)(CryLobbyTaskID taskID, ECryLobbyError error, uint32 privilege, void* pArg);
#define TO_GAME_FROM_LOBBY(...) { CCryLobby* pLobby = static_cast<CCryLobby*>(CCryLobby::GetLobby()); pLobby->LockToGameMutex(); pLobby->GetToGameQueue()->Add(__VA_ARGS__); pLobby->UnlockToGameMutex(); }
typedef CryLockT<CRYLOCK_RECURSIVE> CryLobbyMutex;
#define LOBBY_AUTO_LOCK AUTO_LOCK_T(CryLobbyMutex, ((CCryLobby*)CCryLobby::GetLobby())->GetMutex())
#define FROM_GAME_TO_LOBBY         static_cast<CCryLobby*>(CCryLobby::GetLobby())->GetFromGameQueue()->Add
struct UserID
{
	IEntity* m_obj;
	UserID* UserInvalidID;

	EntityId GetUserID()
	{
		return m_obj->GetId();
	}

};

typedef uint32 LobbyUserIndex;
const LobbyUserIndex LobbyInvalidUserIndex = 0xffffffff;
typedef uint32 LobbySendID;


#define MAX_LOBBY_TASKS             10
struct IMementoManagedThing
{
	virtual ~IMementoManagedThing() {}
	virtual void Release() = 0;
};
struct LobbyMenuItem_t
{
	/* Probably need to change this wording for enum.... */
	enum ELobbyMenuCommand
	{
		k_ELobbyMenuItemUser,
		k_ELobbyMenuItemStartGame,
		k_ELobbyMenuItemToggleReadState,
		k_ELobbyMenuItemLeaveLobby,
		k_ELobbyMenuItemInviteToLobby
	};

	CSteamID m_steamIDUser;		// the user who this is in the lobby
	ELobbyMenuCommand m_eCommand;
	CSteamID m_steamIDLobby;	// set if k_ELobbyMenuItemInviteToLobby	
};
#define MAX_LOBBY_TASK_DATAS        10
// an item in the list of lobbies we've found to display
struct Lobby_t
{
	CSteamID m_steamIDLobby;
	char m_rgchName[256];
};


struct SSystemTime
{
	uint16 m_Year;
	uint16 m_Milliseconds;
	uint8  m_Month;
	uint8  m_DayOfWeek;
	uint8  m_Day;
	uint8  m_Hour;
	uint8  m_Minute;
	uint8  m_Second;
};
enum ECryTCPServiceResult
{
	eCTCPSR_Ok,       //!< No errors in sending data to external site.
	eCTCPSR_Failed    //!< Some sort of error occurred (likely to be a fail in the socket send).
};
enum ECryTCPServiceConnectionStatus
{
	eCTCPSCS_Pending,
	eCTCPSCS_Connected,
	eCTCPSCS_NotConnected,
	eCTCPSCS_NotConnectedUserNotSignedIn,
	eCTCPSCS_NotConnectedDNSFailed,
	eCTCPSCS_NotConnectedConnectionFailed
};
struct STCPServiceData;
typedef _smart_ptr<STCPServiceData> STCPServiceDataPtr;
struct ICryTCPService
{
	// <interfuscator:shuffle>
	virtual ~ICryTCPService(){}

	//! \param isDestructing - is this objects destructor running.
	//! \return Result code.
	virtual ECryTCPServiceResult Terminate(bool isDestructing) = 0;

	//! Has the address resolved?
	//! \return true if the address has resolved, otherwise false.
	virtual bool HasResolved() = 0;

	//! Queue a transaction.
	//! \param pData - transaction.
	//! \return true if transaction was successfully queued, otherwise false
	virtual bool UploadData(STCPServiceDataPtr pData) = 0;

	//! \param delta - time delta from last tick.
	virtual void Tick(CTimeValue delta) = 0;

	//! Increment reference count.
	//! \return Reference count.
	virtual int AddRef() = 0;

	//! Decrement reference count.
	//! \return Reference count.
	virtual int Release() = 0;

	//! Get the current connection status.
	//! \return Status.
	virtual ECryTCPServiceConnectionStatus GetConnectionStatus() = 0;

	//! Get the number of items currently in the data queue.
	//! \return Number of items.
	virtual uint32 GetDataQueueLength() = 0;

	//! Get the total data size currently in the data queue.
	//! \return Number of bytes.
	virtual uint32 GetDataQueueSize() = 0;

	//! Get the server name.
	//! \return Server name.
	virtual const char* GetServerName() = 0;

	//! Get the port.
	//! \return Port
	virtual uint16 GetPort() = 0;

	//! Get the server path.
	//! \return Server path.
	virtual const char* GetURLPrefix() = 0;
	// </interfuscator:shuffle>
};
typedef CMementoMemoryManager::Hdl TMemHdl;
const TMemHdl TMemInvalidHdl = CMementoMemoryManager::InvalidHdl;
class CCryLobby;
typedef void(*LobbyServiceCallback)(ECryLobbyError error, CCryLobby* arg, ECryLobbyService service);
typedef void(*LobbyPrivilegeCallback)(CryLobbyTaskID taskID, ECryLobbyError error, uint32 privilege, void* pArg);
typedef void(*LobbyCheckProfanityCallback)(CryLobbyTaskID taskID, ECryLobbyError error, const char* pString, bool isProfanity, void* pArg);
typedef uint32 CryLobbyServiceTaskID;
const CryLobbyServiceTaskID CryLobbyServiceInvalidTaskID = 0xffffffff;
typedef _smart_ptr<ICryTCPService> ICryTCPServicePtr;
// classes

class CLobbyService : public CMultiThreadRefCount
{
public:
	virtual ~CLobbyService();

	virtual ECryLobbyError    Initialise(ECryLobbyServiceFeatures features, LobbyServiceCallback pCB);
	virtual ECryLobbyError    Terminate(ECryLobbyServiceFeatures features, LobbyServiceCallback pCB);
	virtual void              Tick(CTimeValue tv);
	
	virtual ICryTCPServicePtr GetTCPService(const char* pService);
	virtual ICryTCPServicePtr GetTCPService(const char* pServer, uint16 port, const char* pUrlPrefix);
	virtual ECryLobbyError    GetUserPrivileges(uint32 user, CryLobbyTaskID* pTaskID, LobbyPrivilegeCallback pCB, void* pCBArg);
	virtual ECryLobbyError    CheckProfanity(const char* const pString, CryLobbyTaskID* pTaskID, LobbyCheckProfanityCallback pCb, void* pCbArg) { return eCLE_ServiceNotSupported; };

	virtual void              Tick(CTimeValue tv) = 0;

	virtual void              MakeAddrPCCompatible(TNetAddress& addr);
	virtual void              OnPacket(const TNetAddress& addr, CCryLobbyPacket* pPacket) = 0;
	virtual void              OnError(const TNetAddress& addr, ESocketError error, LobbySendID sendID) = 0;
	virtual void              OnSendComplete(const TNetAddress& addr, LobbySendID sendID) = 0;

	virtual bool              IsDead() const { return false; };

	virtual bool              GetFlag(ECryLobbyServiceFlag flag);

	virtual void              GetSocketPorts(uint16& connectPort, uint16& listenPort);
	virtual void              CancelTask(CryLobbyTaskID lTaskID);

	void                      CreateSocketNT(void);
	void                      FreeSocketNT(void);

protected:
	enum ETask
	{
		eT_GetUserPrivileges,
		eT_LobbyPlatformSpecificTask
	};
	struct  STask
	{
		uint32         user;
		CryLobbyTaskID lTaskID;
		ECryLobbyError error;
		uint32         startedTask;
		void*          pCB;
		void*          pCBArg;
		uint32         dataNum[MAX_LOBBY_TASK_DATAS];
		TMemHdl        dataMem[MAX_LOBBY_TASK_DATAS];
		bool           used;
		bool           running;
	};

	STask*         GetTask(CryLobbyServiceTaskID id) { return &m_tasks[id]; }
	ECryLobbyError StartTask(uint32 eTask, uint32 user, bool startRunning, CryLobbyServiceTaskID* pLSTaskID, CryLobbyTaskID* pLTaskID, void* pCb, void* pCbArg);
	void           FreeTask(CryLobbyServiceTaskID lsTaskID);
	void           UpdateTaskError(CryLobbyServiceTaskID lsTaskID, ECryLobbyError error);
	void           StartTaskRunning(CryLobbyServiceTaskID lsTaskID);
	void           StopTaskRunning(CryLobbyServiceTaskID lsTaskID);
	void           EndTask(CryLobbyServiceTaskID lsTaskID);
	ECryLobbyError CreateTaskParamMem(CryLobbyServiceTaskID lsTaskID, uint32 param, const void* pParamData, size_t paramDataSize);

	// In the future it would be good to move the common task handling code into CCryLobbyService and the common platform task handling code into the platform lobby services.
	// Because of this tasks should always be accessed via GetTask() as m_tasks will be removed when the platform task data gets added.
	STask                    m_tasks[MAX_LOBBY_TASKS];
	CCryLobby*               m_pLobby;
	ECryLobbyService         m_service;
#if USE_CRY_TCPSERVICE
	CCryTCPServiceFactoryPtr m_pTCPServiceFactory;
#endif // USE_CRY_TCPSERVICE
	uint32                   m_lobbyServiceFlags;

private:

};

class CMementoMemoryManager 
{
	friend class CMementoStreamAllocator;

public:
	// hack for arithmetic alphabet stuff
	uint32                arith_zeroSizeHdl;
	IMementoManagedThing* pThings[64];

	// who's using pThings:
	//   0 - arith row sym cache
	//   1 - arith row low cache

	CMementoMemoryManager(const string& name);
	~CMementoMemoryManager();

	typedef uint32 Hdl;
	static const Hdl InvalidHdl = ~Hdl(0);

	void*        AllocPtr(size_t sz, void* callerOverride = 0);
	void         FreePtr(void* p, size_t sz);
	Hdl          AllocHdl(size_t sz, void* callerOverride = 0);
	Hdl          CloneHdl(Hdl hdl);
	void         ResizeHdl(Hdl hdl, size_t sz);
	void         FreeHdl(Hdl hdl);
	void         AddHdlToSizer(Hdl hdl, ICrySizer* pSizer);
	ILINE void*  PinHdl(Hdl hdl) const { return CMementoMemoryManagerAllocator::GetAllocator()->PinHdl(hdl); }
	ILINE size_t GetHdlSize(Hdl hdl) const { return CMementoMemoryManagerAllocator::GetAllocator()->GetHdlSize(hdl); }

	void         GetMemoryStatistics(ICrySizer* pSizer, bool countingThis = false);

	static void  DebugDraw();
	static void  Tick();

private:
	class CMementoMemoryManagerAllocator
	{
		struct SPoolStats
		{
			SPoolStats() : allocated(0), used(0) {}
			size_t allocated;
			size_t used;

			float  GetWastePercent() const
			{
				return allocated ? 100.0f * (1.0f - float(used) / float(allocated)) : 0.0f;
			}
		};

#if MMM_USE_BUCKET_ALLOCATOR
		typedef BucketAllocator<BucketAllocatorDetail::DefaultTraits<MMM_BUCKET_ALLOCATOR_SIZE, BucketAllocatorDetail::SyncPolicyUnlocked, false>> MMMBuckets;

		static MMMBuckets m_bucketAllocator;
		size_t            m_bucketTotalRequested;
		size_t            m_bucketTotalAllocated;

		struct SHandleData
		{
			void*  p;
			size_t size;
			size_t capacity;
		};

#else
		struct SHandleData
		{
			void*  p;
			size_t size;
			size_t capacity;
		};

#endif

#if !defined(PURE_CLIENT)
		_smart_ptr<IGeneralMemoryHeap> m_pGeneralHeap;
		size_t              m_generalHeapTotalRequested;
		size_t              m_generalHeapTotalAllocated;
#endif

	public:
		CMementoMemoryManagerAllocator();
		~CMementoMemoryManagerAllocator();

		static CMementoMemoryManagerAllocator* GetAllocator() { return m_allocator; }
		static void                            AddCMementoMemoryManager();
		static void                            RemoveCMementoMemoryManager();

		void                                   Tick();
		Hdl                                    AllocHdl(size_t sz);
		void                                   FreeHdl(Hdl hdl);
		void*                                  AllocPtr(size_t sz);
		void                                   FreePtr(void* p, size_t sz);
		void                                   ResizeHdl(Hdl hdl, size_t sz);
		void                                   InitHandleData(SHandleData& hd, size_t sz);
		void                                   DebugDraw(int x, int& y, size_t& totalAllocated);
		ILINE void*                            PinHdl(Hdl hdl) const
		{
			hdl = UnprotectHdl(hdl);
			return (hdl != InvalidHdl) ? m_handles[hdl].p : NULL;
		}
		ILINE size_t GetHdlSize(Hdl hdl) const
		{
			hdl = UnprotectHdl(hdl);
			return (hdl != InvalidHdl) ? m_handles[hdl].size : 0;
		}

#if LOG_BUCKET_ALLOCATOR_HIGH_WATERMARK
		size_t m_bucketHighWaterMark;
		size_t m_generalHeapHighWaterMark;
#endif // LOG_BUCKET_ALLOCATOR_HIGH_WATERMARK

	private:
		ILINE Hdl ProtectHdl(Hdl x) const
		{
#if !CRY_PLATFORM_APPLE && !CRY_PLATFORM_LINUX && !CRY_PLATFORM_ORBIS && CRYNETWORK_RELEASEBUILD
			if (x != InvalidHdl)
			{
				return (x << 1) ^ ((uint32)UINT_PTR(this) + 1);     // ensures 0xFFFFFFFF cannot be a valid result (this will always be at least 4 byte aligned)
			}
#endif
			return x;
		}

		ILINE Hdl UnprotectHdl(Hdl x) const
		{
#if !CRY_PLATFORM_APPLE && !CRY_PLATFORM_LINUX && !CRY_PLATFORM_ORBIS && CRYNETWORK_RELEASEBUILD
			if (x != InvalidHdl)
			{
				return (x ^ ((uint32)UINT_PTR(this) + 1)) >> 1;
			}
#endif
			return x;
		}

		std::vector<SHandleData>               m_handles;
		std::vector<uint32>                    m_freeHandles;

		static CMementoMemoryManagerAllocator* m_allocator;
		static int                             m_numCMementoMemoryManagers;
#if MMM_MUTEX_ENABLE
		static CryLockT<CRYLOCK_RECURSIVE>     m_mutex;
#endif
	};

	size_t m_totalAllocations;
	string m_name;

#if MMM_CHECK_LEAKS
	std::map<void*, void*>  m_ptrToAlloc;
	std::map<uint32, void*> m_hdlToAlloc;
	std::map<void*, uint32> m_allocAmt;
#endif

#if ENABLE_NETWORK_MEM_INFO
	typedef std::list<CMementoMemoryManager*> TManagers;
	static TManagers* m_pManagers;
#endif
};

class CSteamLobby : public CLobbyService
{

public:

protected:

private:

};

class CSteamLobbySystem : public UserID
{

public:
	/* Looks like this would work if not let me know TODO: Maybe change pointer return to uint32 */
	virtual UserID  GetUserID(uint32 user) { return UserInvalidID->GetUserID; }
	virtual ECryLobbyError GetSystemTime(uint32 user, SSystemTime* pSystemTime);

	virtual void           OnPacket(const TNetAddress& addr, CCryLobbyPacket* pPacket);
	virtual void           OnError(const TNetAddress& addr, ESocketError error, LobbySendID sendID);
	virtual void           OnSendComplete(const TNetAddress& addr, LobbySendID sendID);

	virtual void           GetSocketPorts(uint16& connectPort, uint16& listenPort);

	void                   InviteAccepted(uint32 user, CrySessionID id);
protected:


private:
	// Create the stuff that needs to private below since CSteamLobby would only need it
	CSteamID m_steamIDLobby;

	// user state change handler
	STEAM_CALLBACK(CSteamLobbySystem, OnPersonaStateChange, PersonaStateChange_t, m_CallbackPersonaStateChange);

	// lobby state change handler
	STEAM_CALLBACK(CSteamLobbySystem, OnLobbyDataUpdate, LobbyDataUpdate_t, m_CallbackLobbyDataUpdate);
	STEAM_CALLBACK(CSteamLobbySystem, OnLobbyChatUpdate, LobbyChatUpdate_t, m_CallbackChatDataUpdate);
};


