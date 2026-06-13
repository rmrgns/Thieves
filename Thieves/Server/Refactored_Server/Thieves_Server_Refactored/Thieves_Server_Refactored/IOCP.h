#pragma once
#include "CoroutineTypes.h"
#include "Session.h"
#include <thread>
#include <array>
#include <Windows.h>
#include <WinSock2.h>
#include <vector>
#include <optional>
#include "protocol.h"
#include "SessionManager.h"



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

protected:

	SOCKET m_Socket;
	HANDLE m_Handle;
	int m_WokerNum;
	std::vector <std::jthread> m_WorkerThreads;

	SOCKET c_Socket;

	LPFN_ACCEPTEX acceptFunc = nullptr;
	std::array<AcceptContext, 50> acceptCtxs;

};

