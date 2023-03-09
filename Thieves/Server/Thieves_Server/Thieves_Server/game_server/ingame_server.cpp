
#include "ingame_server.h"
#include "packet_manager.h"
#include "pch.h"
using namespace std;

// 생성자 함수 PacketManager를 초기화
InGameServer::InGameServer()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();
}

// 소멸자 함수
InGameServer::~InGameServer()
{
}

// 클라이언트와 연결이 성공할시 호출하는 함수 PacketManager에 연결된 클라 정보 전달
bool InGameServer::OnAccept(EXP_OVER* exp_over)
{
	m_PacketManager->ProcessAccept(m_hiocp, m_s_socket, exp_over);
	return true;
}

// 클라 데이터를 수신시 호출되는 함수 수신된 데이터 PacketManager에 수신된 데이터 전달
bool InGameServer::OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes)
{
	m_PacketManager->ProcessRecv(c_id, exp_over, num_bytes);
	return true;
}

// 비동기 입출력 완료 루틴에서 이벤트 발생시 호출되는 함수
// 이벤트 종류에 따라 다르게 동작
void InGameServer::OnEvent(int c_id, EXP_OVER* exp_over)
{
	/*if (false == m_PacketManager->IsRoomInGame(exp_over->room_id))
	{
		delete exp_over;
		return;
	}*/
	
}

// 클라 연결을 끊을 때 호출되는 함수
// PacketManager에 연결을 끊은 클라 정보를 전달
void InGameServer::Disconnect(int c_id)
{
	m_PacketManager->Disconnect(c_id);
}

// 타이머 스레드에서 주기적으로 호출되는 함수
// PacketManager에서 타이머 이벤트 처리
void InGameServer::DoTimer(HANDLE hiocp)
{
	m_PacketManager->ProcessTimer(hiocp);
}

// 타이머 쓰레드 생성 함수
void InGameServer::CreateTimer()
{
	m_worker_threads.emplace_back(std::thread(&InGameServer::DoTimer, this, m_hiocp));

}

// 게임 서버를 시작하는 함수 서버시작, 타이머쓰레드 생성, DB쓰레드 생성, 쓰레드 조인 동작
void InGameServer::Run()
{


	StartServer();
	CreateTimer();
	m_PacketManager->CreateDBThread();
	JoinThread();
	m_PacketManager->JoinDBThread();
}

// 서버 종료 함수
void InGameServer::End()
{
	m_PacketManager->End();

	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}