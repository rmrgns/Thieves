#include "IOCP.hpp"
#include <WinSock2.h>
#include <ws2def.h>
#include <Windows.h>
#include <iostream>
#include <ostream>
#include <MSWSock.h>
#include "CoroutineTypes.h"

IOCP::IOCP() : m_Handle(INVALID_HANDLE_VALUE), m_Socket(INVALID_SOCKET) 
{

}

IOCP::~IOCP()
{
	WSACleanup();
}

bool IOCP::Init(const int workerNum)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;

	// Listen을 하기위한 소켓
	m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == m_Socket)
	{
		ErrorDisplay(WSAGetLastError());
		return false;
	}

	m_WokerNum = workerNum;

	return true;
}

bool IOCP::BindListen(const int portNum)
{
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

	m_Handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0); // iocp 초기화
	
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_Socket), m_Handle, 0, 0); // iocp 생성

	// accept용 소켓 생성
	SOCKET cSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	ZeroMemory(acceptCtx.GetOverLapped(), sizeof(acceptCtx.GetOverLapped()));

	AcceptEx(m_Socket, cSocket, reinterpret_cast<SendContext*>(&acceptCtx)->GetSendBuffer(), 0, sizeof(PSOCKADDR_IN) + 16,
		sizeof(PSOCKADDR_IN) + 16, 0, acceptCtx.GetOverLapped());

	std::wcout << "Accept Called.\n";
	return true;

}

void IOCP::ErrorDisplay(int errNum)
{}

bool IOCP::Start()
{
	return false;
}

int IOCP::GetEmptySessionId()
{
	return 0;
}

void IOCP::JoinThreads()
{}

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
		DWORD numBytes;
		LONG64 iocpKey;
		WSAOVERLAPPED* pOver;

		BOOL result = GetQueuedCompletionStatus(m_Handle, &numBytes, (PULONG_PTR)&iocpKey, &pOver, INFINITE);
		IOContext* ctx = reinterpret_cast<IOContext*>(pOver);

		if (FALSE == result || (numBytes == 0 && ctx && false == ctx->IsSend()))
		{
			int errNum = WSAGetLastError();
			std::cout << "GQCS ERROR : ";
			ErrorDisplay(errNum);
			std::cout << "\n";

			// 에러가 났거나 통신이 끊겼으면, disconnect 해줘야 함.
			// 여기서 각 세션으로 가는게 아니라,
			// 받은 bytes 수를 0으로 설정해주는 것으로
			// 코루틴 내부에서 스스로 멈추도록 설정해 줌

			if (ctx && ctx->GetHandle())
			{
				ctx->setBytesTransferred(0);
				ctx->GetHandle()->resume();
			}
			continue;
		}

		if (ctx == &acceptCtx)
		{
			int newSessionId = 0;
		}
	}
}