#pragma once
#include "CoroutineTypes.h"
#include "Session.hpp"
#include <thread>
#include <array>
#include <Windows.h>
#include <WinSock2.h>
#include <vector>
#include "protocol.hpp"

__declspec(align(16)) struct SessionNode {
	SLIST_ENTRY ItemEntry; 
	int SessionId;         
};

class IOCP
{
public:
	IOCP();
	~IOCP();
	bool Init(const int);
	bool BindListen(const int);

	virtual bool OnAccept(IOContext* ctx) { return false; }
	virtual bool OnRecv(int id, IOContext* ctx) { return false; }
	virtual bool OnEvent(int id, IOContext* ctx) { return false; }
	static void ErrorDisplay(int errNum);
	virtual bool Start();

	int GetEmptySessionId();

	void JoinThreads();
	void CreateWorker();
	void WorkerThread();

protected:
	IOContext acceptCtx;
	SOCKET m_Socket;
	HANDLE m_Handle;
	int m_WokerNum;
	std::vector <std::thread> m_WorkerThreads;

	std::array<Session, MAX_USER> sessions;
};

