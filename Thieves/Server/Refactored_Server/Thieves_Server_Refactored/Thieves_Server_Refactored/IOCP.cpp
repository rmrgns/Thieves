#include "IOCP.hpp"
#include <WinSock2.h>
#include <ws2def.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <MSWSock.h>
#include "CoroutineTypes.h"
#include <thread>
#include "protocol.hpp"
#include "State.hpp"

IOCP::IOCP() : m_Handle(INVALID_HANDLE_VALUE), m_Socket(INVALID_SOCKET) 
{

}

IOCP::~IOCP()
{
	WSACleanup();
}

bool IOCP::Init(const int workerNum, const int portNum)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;

	m_Handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0); // iocp 초기화

	// Listen을 하기위한 소켓
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_Socket), m_Handle, 0, 0); // iocp 생성

	if (INVALID_SOCKET == m_Socket)
	{
		ErrorDisplay(WSAGetLastError());
		return false;
	}

	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(portNum);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int result = bind(m_Socket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

	if (0 != result)
	{
		ErrorDisplay(WSAGetLastError());
		return false;
	}

	result = listen(m_Socket, SOMAXCONN);

	if (0 != result) {
		ErrorDisplay(WSAGetLastError());
		return false;
	}

	DWORD bytes = 0;
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	result = WSAIoctl(
		m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx),
		&acceptFunc, sizeof(acceptFunc),
		&bytes, nullptr, nullptr);

	if (result == SOCKET_ERROR)
	{
		std::cout << "LPFN_ACCEPTEX ERROR.\n";
		return false;
	}

	InitializeSessions();


	m_WokerNum = std::thread::hardware_concurrency();

	return true;
}

void IOCP::PostAccept(AcceptContext* ctx)
{
	std::cout << "Post Accept Start.\n";

	ctx->MakeNewSocket();

	std::cout << "AcceptContext Make New Socket. : " << *ctx->GetSocket() << "\n";

	ZeroMemory(ctx->GetOverLapped(), sizeof(*ctx->GetOverLapped()));

	DWORD bytes = 0;
	int addrSize = sizeof(SOCKADDR_IN) + 16;

	if (FALSE == acceptFunc(
		m_Socket,
		*ctx->GetSocket(),
		ctx->GetBuffer(),
		0, addrSize, addrSize,
		&bytes, ctx->GetOverLapped()
	))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			std::cout << "PostAcceptError. \n";
		}
	}

	std::wcout << "accept Complete.\n";

}

void IOCP::ErrorDisplay(int errNum)
{
	WCHAR* lpMsgBuf = nullptr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
}

bool IOCP::Start()
{
	CreateWorker();

	for (int i = 0; i < acceptCtxs.size(); ++i)
	{
		PostAccept(&acceptCtxs[i]);
	}

	return true;
}

void IOCP::InitializeSessions()
{
	InitializeSListHead(&freeSessionList);

	for (int i = 0; i < MAX_USER; ++i)
	{
		sessionNodes[i].SessionId = i;

		sessions[i].SetStateCallback([this](int id) {
			this->ReturnSessionId(id);
		});

		InterlockedPushEntrySList(&freeSessionList, &(sessionNodes[i].ItemEntry));
	}

	std::cout << "Session Initialized. \n";
}

int IOCP::GetEmptySessionId()
{

	PSLIST_ENTRY entry = InterlockedPopEntrySList(&freeSessionList);

	std::cout << "GetEmptySessionId. \n";

	if (entry == nullptr) {
		return -1; // 꽉 찼음
	}

	SessionNode* node = reinterpret_cast<SessionNode*>(entry);
	
	std::cout << "EmptySessionId is " << node->SessionId << "\n";

	return node->SessionId;
}

void IOCP::ReturnSessionId(int id)
{
	sessions[id].SetState(static_cast<int>(S_STATE::ST_FREE));

	InterlockedPushEntrySList(&freeSessionList, &(sessionNodes[id].ItemEntry));

	std::cout << "ReturnSessionId " << id << "\n";
}

void IOCP::JoinThreads()
{
	for (auto& worker : m_WorkerThreads)
	{
		worker.join();
	}
}

void IOCP::CreateWorker()
{
	for (int i = 0; i < m_WokerNum; ++i)
	{
		m_WorkerThreads.emplace_back([this]() {
			WorkerThread();
			});
	}
}

void IOCP::WorkerThread()
{
	while (true) {
		DWORD numBytes = 0;
		LONG64 iocpKey = 0;
		WSAOVERLAPPED* pOver = nullptr;

		BOOL result = GetQueuedCompletionStatus(m_Handle, &numBytes, (PULONG_PTR)&iocpKey, &pOver, INFINITE);
		IOContext* ctx = reinterpret_cast<IOContext*>(pOver);

		std::cout << "GQCS END. Key is " << iocpKey << ".\n";

		std::cout << ctx << "\n";

		bool isAccept = (ctx >= &acceptCtxs.front() && ctx <= &acceptCtxs.back());

		if (FALSE == result || (numBytes == 0 && ctx && false == ctx->IsSend() && !isAccept))
		{
			// 에러가 났거나 통신이 끊겼으면, disconnect 해줘야 함.
			// 여기서 각 세션으로 가는게 아니라,
			// 받은 bytes 수를 0으로 설정해주는 것으로
			// 코루틴 내부에서 스스로 멈추도록 설정해 줌

			int errNum = WSAGetLastError();
			std::cout << "GQCS ERROR : ";
			if (numBytes == 0)
			{
				std::cout << " numBytes is 0 ";
			}
			else
			{
				ErrorDisplay(errNum);
			}
			std::cout << "\n";

			if (ctx != nullptr && *ctx->GetHandle())
			{
				ctx->setBytesTransferred(0);
				ctx->GetHandle()->resume();
			}

			continue;
		}

		if (isAccept)
		{
			AcceptContext* curAcceptCtx = static_cast<AcceptContext*>(ctx);
			int newId = GetEmptySessionId();

			if (newId != -1)
			{
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(*curAcceptCtx->GetSocket()), m_Handle, newId, 0);
				sessions[newId].Init(newId, *curAcceptCtx->GetSocket());
				sessions[newId].Run();
			}
			else
			{
				closesocket(*curAcceptCtx->GetSocket());
			}

			PostAccept(curAcceptCtx);
			continue;
		}

		if (ctx->IsSend())
		{
			delete reinterpret_cast<SendContext*>(ctx);
		}

		if (*ctx->GetHandle())
		{
			ctx->setBytesTransferred(numBytes);
			ctx->GetHandle()->resume();
		}
	}
}


