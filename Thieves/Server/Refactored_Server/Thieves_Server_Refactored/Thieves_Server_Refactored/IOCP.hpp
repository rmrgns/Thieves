#pragma once
#include "CoroutineTypes.h"
#include "Session.hpp"
#include <thread>
#include <array>
#include <Windows.h>
#include <WinSock2.h>
#include <vector>
#include <optional>
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
	bool Init(const int workerNum, const int portNum);

	virtual bool OnAccept(IOContext* ctx) { return false; }
	virtual bool OnRecv(int id, IOContext* ctx) { return false; }
	virtual bool OnEvent(int id, IOContext* ctx) { return false; }
	static void ErrorDisplay(int errNum);
	virtual bool Start();

	void CreateWorker();
	void WorkerThread();

	void PostAccept(AcceptContext*);

	void InitializeSessions();
	std::optional<int> GetEmptySessionId();
	void ReturnSessionId(int id);

protected:

	SOCKET m_Socket;
	HANDLE m_Handle;
	int m_WokerNum;
	std::vector <std::jthread> m_WorkerThreads;

	SOCKET c_Socket;

	LPFN_ACCEPTEX acceptFunc = nullptr;
	std::array<AcceptContext, 50> acceptCtxs;

	std::array<Session, MAX_USER> sessions;

	// Interlocked Singly Linked Lists -> SList를 활용함
	// 사실상 락프리 스택 같은 느낌
	// 16바이트로 정렬한 뒤, 카운터를 활용해서 ABBA 문제도 해결해 준다고 함.
	__declspec(align(16)) SLIST_HEADER freeSessionList;
	__declspec(align(16)) SessionNode sessionNodes[MAX_USER];
};

