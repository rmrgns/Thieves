#include "pch.h"
#include "ingame_server.h"
#include "packet_manager.h"
#include <Windows.h>
#include <WinSock2.h>
#include <memory>
#include <thread>
#include "../define.h"
#include "../state.h"

using namespace std;

// PacketManager 초기화
InGameServer::InGameServer()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();
}

// PacketManager 소멸자
InGameServer::~InGameServer()
{
}

// 클라이언트가 연결되었을 때 호출되는 함수로, PacketManager의 ProcessAccept 함수를 호출하여 클라이언트 연결을 처리한다.q
bool InGameServer::OnAccept(IOContext* ctx)
{
	m_PacketManager->ProcessAccept(m_hiocp, m_s_socket, ctx);
	return true;
}

// 클라이언트로부터 데이터를 수신했을 때 호출되는 함수로, PacketManager의 ProcessRecv 함수를 호출하여 수신된 데이터를 처리한다.
bool InGameServer::OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes)
{
	m_PacketManager->ProcessRecv(c_id, exp_over, num_bytes);
	return true;
}

// 타이머로부터 이벤트가 발생했을 때 호출되는 함수로, PacketManager의 다양한 함수를 호출하여 이벤트를 처리한다.
void InGameServer::OnEvent(int c_id, EXP_OVER* exp_over)
{
	
	//if (false == m_PacketManager->IsRoomInGame(exp_over->room_id))
	//{
	//	delete exp_over;
	//	return;
	//}
	
	switch (exp_over->_comp_op)
	{
	case COMP_OP::OP_NPC_SPAWN: {
		m_PacketManager->SpawnNPC(exp_over->room_id);
		delete exp_over;
		break;
	}
	case COMP_OP::OP_NPC_MOVE: {
		m_PacketManager->DoNpcMove(exp_over->room_id, c_id);
		delete exp_over;
		break;
	}
	case COMP_OP::OP_STUN_END:
		m_PacketManager->ProcessStunEnd(c_id);
		delete exp_over;
		break;
	case COMP_OP::OP_NPC_ATTACK:{
		m_PacketManager->DoEnemyAttack(c_id, exp_over->target_id, exp_over->room_id);
		delete exp_over;
		break;
	}
	case COMP_OP::OP_INVINCIBLE_END:
		m_PacketManager->ProcessInvincibleEnd(c_id);
		delete exp_over;
		break;
	case COMP_OP::OP_TIMER_START:
		m_PacketManager->ProcessTimerStart(c_id);
		delete exp_over;
		break;
	case COMP_OP::OP_OPEN_SAFE:
		m_PacketManager->ProcessOpenSafe(c_id);
		delete exp_over;
		break;
	case COMP_OP::OP_OPEN_ESCAPE:
		m_PacketManager->ProcessOpenEscape(c_id);
		delete exp_over;
		break;
	case COMP_OP::OP_OPEN_SPECIAL_ESCAPE:
		m_PacketManager->ProcessOpenSpecialEscape(c_id);
		delete exp_over;
		break;
	case COMP_OP::OP_NPC_TIMER_SPAWN: 
		m_PacketManager->SpawnNPCTime(c_id, exp_over->room_id);
		delete exp_over;
		break;
	
	case COMP_OP::OP_ROOM_END: 
		m_PacketManager->PlayerMoveToLobby(c_id);
		delete exp_over;
	
	defalut:
		break;
	}
	
}

void InGameServer::Disconnect(int c_id)
{
	m_PacketManager->Disconnect(c_id);
}

void InGameServer::DoTimer(HANDLE hiocp)
{
	m_PacketManager->ProcessTimer(hiocp);
}

void InGameServer::CreateTimer()
{
	m_worker_threads.emplace_back(std::thread(&InGameServer::DoTimer, this, m_hiocp));

}


void InGameServer::Run()
{
	StartServer();
	CreateTimer();
	m_PacketManager->CreateDBThread();
	JoinThread();
	m_PacketManager->JoinDBThread();
}

void InGameServer::End()
{
	m_PacketManager->End();

	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}