// IOCP서버
// 클라연결 연결, 이후 클라와 송수신 작업

#include "pch.h"
#include "iocp_server.h"
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
	char	accept_buf[sizeof(SOCKADDR_IN) * 2 + 32 + 100];

	ZeroMemory(&accept_ex._wsa_over, sizeof(accept_ex._wsa_over));
	*(reinterpret_cast<SOCKET*>(&accept_ex._net_buf)) = c_socket;
	accept_ex._comp_op = COMP_OP::OP_ACCEPT;

	AcceptEx(m_s_socket, c_socket, accept_buf, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &accept_ex._wsa_over);
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
	for (;;) {
		// iocp 대기중인 작업
		DWORD num_byte;
		LONG64 iocp_key;
		WSAOVERLAPPED* p_over;
		BOOL ret = GetQueuedCompletionStatus(m_hiocp, &num_byte, (PULONG_PTR)&iocp_key, &p_over, INFINITE);

		int client_id = static_cast<int>(iocp_key);
		EXP_OVER* exp_over = reinterpret_cast<EXP_OVER*>(p_over);
		if (FALSE == ret) {
			int err_no = WSAGetLastError();
			cout << "GQCS Error : ";
			error_display(err_no);
			cout << endl;
			Disconnect(client_id);
			if (exp_over->_comp_op == COMP_OP::OP_SEND)
				delete exp_over;
			continue;
		}

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
	}
	cout << "Worker" << endl;

}

// 에러확인
void IOCPServer::error_display(int err_no)
{
	WCHAR* lpMsgBuf;
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
