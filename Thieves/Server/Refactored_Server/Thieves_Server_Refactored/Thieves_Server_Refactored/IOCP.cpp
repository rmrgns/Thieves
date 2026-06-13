#include "IOCP.h"
#include <WinSock2.h>
#include <ws2def.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <MSWSock.h>
#include "CoroutineTypes.h"
#include <thread>
#include "SendContextManager.h"

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

	SOCKADDR_IN serverAddr{
		.sin_family = AF_INET,
		.sin_port = htons(portNum),
		.sin_addr = {.S_un = {.S_addr = htonl(INADDR_ANY)}}
	};
		
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

	SessionManager::GetInst().InitializeSessions();

	m_WokerNum = std::thread::hardware_concurrency();

	return true;
}

void IOCP::PostAccept(AcceptContext* ctx)
{
	ctx->MakeNewSocket();
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
			std::cout << "PostAcceptError : ";
			ErrorDisplay(WSAGetLastError());
			std::cout << "\n";
		}
	}
}

void IOCP::ErrorDisplay(int errNum)
{
	WCHAR* lpMsgBuf = nullptr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, errNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf;
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

		bool isAccept = (ctx >= &acceptCtxs.front() && ctx <= &acceptCtxs.back());

		if (FALSE == result || (numBytes == 0 && ctx && false == ctx->IsSend() && !isAccept))
		{
			// 에러가 났거나 통신이 끊겼으면, disconnect 해줘야 함.
			// 여기서 각 세션으로 가는게 아니라,
			// 받은 bytes 수를 0으로 설정해주는 것으로
			// 코루틴 내부에서 스스로 멈추도록 설정해 줌

			int errNum = WSAGetLastError();
			std::cout << "GQCS ERROR [" << errNum << "] : ";
			ErrorDisplay(errNum);
			if(numBytes == 0) std::cout << "and numBytes is 0 \n";

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

			if (auto newId = SessionManager::GetInst().GetEmptySessionId(); newId.has_value())
			{
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(*curAcceptCtx->GetSocket()), m_Handle, newId.value(), 0);
				SessionManager::GetInst().GetSession(newId.value())->Init(newId.value(), *curAcceptCtx->GetSocket());
				SessionManager::GetInst().GetSession(newId.value())->Run();
				DEBUG_LOG("[" << newId.value() << "] is Accepted. \n");
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
			SendContext* sendCtx = static_cast<SendContext*>(ctx);
			SendContextManager::GetInst().Push(sendCtx);
			continue;
		}

		if (*ctx->GetHandle())
		{
			ctx->setBytesTransferred(numBytes);
			ctx->GetHandle()->resume();
		}
	}
}


