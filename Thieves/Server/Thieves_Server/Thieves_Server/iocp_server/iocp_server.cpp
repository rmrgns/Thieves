// IOCP서버
// 클라연결 연결, 이후 클라와 송수신 작업

#include "pch.h"
#include "iocp_server.h"
#include <ws2def.h>
#include <MSWSock.h>
#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include <ostream>
#include <coroutine>
#include <span>
#include "../define.h"
#include "../state.h"
#include "../IOContext.h"
#include "../SendContext.h"
using namespace std;

IOCPServer::IOCPServer()
{
}

IOCPServer::~IOCPServer()
{
	WSACleanup();
}

// 윈도우소켓 초기화, 소켓 생성
bool IOCPServer::Init(const int worker_num)
{
	WSADATA WSAData;
	if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
		return false;
	m_s_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_s_socket)
	{
		error_display(WSAGetLastError());
		return false;
	}
	m_worker_num = worker_num;
	return true;
}

// 생성한 소켓 바인딩 후 LISTEN, IOCP포트 생성 및 클라이언트 연결대기
bool IOCPServer::BindListen(const int port_num)
{
	SOCKADDR_IN server_addr;
	ZeroMemory(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_num);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int retval = bind(m_s_socket, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
	if (0 != retval) {
		error_display(WSAGetLastError());
		return false;
	}

	retval = listen(m_s_socket, SOMAXCONN);
	if (0 != retval) {
		error_display(WSAGetLastError());
		return false;
	}

	m_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(m_s_socket), m_hiocp, 0, 0);

	SOCKET c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	// Accept도 IOContext를 활용해, 코루틴으로 처리할 수 있도록 변경.

	// accept용 데이터 초기화
	ZeroMemory(acceptCtx.GetOverLapped(), sizeof(acceptCtx.GetOverLapped()));

	// sizeof(SOCKADDR_IN) 는 주소의 크기.

	AcceptEx(m_s_socket, c_socket, reinterpret_cast<SendContext*>(&acceptCtx)->GetSendBuffer(), 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, 0, acceptCtx.GetOverLapped());
	cout << "Aceept Called\n";
	return true;
}

// IOCP 작업자 쓰레드 생성
void IOCPServer::CreateWorker()
{
	for (int i = 0; i < m_worker_num; ++i)
		m_worker_threads.emplace_back([this]() {Worker(); });
}

// IOCP 쓰레드
void IOCPServer::Worker()
{
	// iocp 대기중인 작업


	while (true) {
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL ret = GetQueuedCompletionStatus(m_hiocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);

		int client_id = static_cast<int>(iocp_key);
		
		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			cout << "GQCS Error : ";
			error_display(err_no);
			cout << endl;
			Disconnect(client_id);
			continue;
		}

		
		// C++ 17 이전까지는 코루틴이 없었기 때문에 switch-case문으로
		// 작업을 구분해서 처리했으나, C++20 이후 부터 코루틴을 활용하는 것이 좋아보임.

		// IOContext가 생김으로서, 기존의 exp_over 구조체를 대신해줌.
		// IOContext는 WSAOVERLAPPED 구조체를 포함하고 있기 때문에,
		// exp_over 대신 IOContext로 reinterpret_cast해서 사용하면 됨.
		auto* ctx = reinterpret_cast<IOContext*>(p_over);

		ctx->setBytesTransferred(num_byte);

		// 아래와 같이 resume만 호출하면, IOContext에 저장된 코루틴 핸들을 통해 해당 코루틴이 다시 실행됨.
		if (ctx->GetHandle() != nullptr) { ctx->GetHandle()->resume(); }

		// 이전에는 아래에서 OP 타입을 나누어 일일이 확인해야 했지만,
		// IOContext를 활용하면, 코루틴 핸들만 resume해주면 되기 때문에, 코드가 훨씬 간결해짐.

		// 기존에 하던 Recv, ProcessPacket, OpRecv 등은
		// 함수 하나로 통일 가능.

		if (ctx == &acceptCtx) {
			OnAccept(&acceptCtx);
			cout << "Accept End.\n";
		}

		if (ctx->IsSend()) {
			delete reinterpret_cast<SendContext*>(ctx);
			continue;
		}

		if (ctx->GetHandle() != nullptr) {
			ctx->setBytesTransferred(num_byte);
			ctx->GetHandle()->resume();
		}

		/* 이전 코드
		switch (exp_over->_comp_op) {
		case COMP_OP::OP_RECV: {
			if (false == OnRecv(client_id, exp_over, num_byte))
				continue;
			break;
		}
		case COMP_OP::OP_SEND: {
			if (num_byte != exp_over->_wsa_buf.len) {
				Disconnect(client_id);
				cout << "Send :dis" << endl;
			}
			delete exp_over;
			break;
		}
		case COMP_OP::OP_ACCEPT: {
			OnAccept(exp_over);
			cout << "Accept Completed.\n";

			break;
		}
		default:
			OnEvent(client_id, exp_over);
			break;
		}
		*/
	}
	cout << "Worker" << endl;

}

// 에러확인
void IOCPServer::error_display(int err_no)
{
	WCHAR* lpMsgBuf = nullptr;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, 0);
	std::wcout << lpMsgBuf << std::endl;
	//while (true);
	LocalFree(lpMsgBuf);
}

// IOCP 쓰레드 생성
bool IOCPServer::StartServer()
{
	CreateWorker();

	return true;
}

// 쓰레드 종료
void IOCPServer::JoinThread()
{
	for (auto& th : m_worker_threads)
		th.join();

}
