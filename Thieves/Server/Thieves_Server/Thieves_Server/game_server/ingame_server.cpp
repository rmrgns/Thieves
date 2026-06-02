#include "pch.h"
#include "ingame_server.h"
#include "packet_manager.h"

using namespace std;

// ������ �Լ� PacketManager�� �ʱ�ȭ
InGameServer::InGameServer()
{
	m_PacketManager = std::make_unique<PacketManager>();
	m_PacketManager->Init();
}

// �Ҹ��� �Լ�
InGameServer::~InGameServer()
{
}

// Ŭ���̾�Ʈ�� ������ �����ҽ� ȣ���ϴ� �Լ� PacketManager�� ����� Ŭ�� ���� ����
bool InGameServer::OnAccept(EXP_OVER* exp_over)
{
	m_PacketManager->ProcessAccept(m_hiocp, m_s_socket, exp_over);
	return true;
}

// Ŭ�� �����͸� ���Ž� ȣ��Ǵ� �Լ� ���ŵ� ������ PacketManager�� ���ŵ� ������ ����
bool InGameServer::OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes)
{
	m_PacketManager->ProcessRecv(c_id, exp_over, num_bytes);
	return true;
}

// �񵿱� ����� �Ϸ� ��ƾ���� �̺�Ʈ �߻��� ȣ��Ǵ� �Լ�
// �̺�Ʈ ������ ���� �ٸ��� ����
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

// Ŭ�� ������ ���� �� ȣ��Ǵ� �Լ�
// PacketManager�� ������ ���� Ŭ�� ������ ����
void InGameServer::Disconnect(int c_id)
{
	m_PacketManager->Disconnect(c_id);
}

// Ÿ�̸� �����忡�� �ֱ������� ȣ��Ǵ� �Լ�
// PacketManager���� Ÿ�̸� �̺�Ʈ ó��
void InGameServer::DoTimer(HANDLE hiocp)
{
	m_PacketManager->ProcessTimer(hiocp);
}

// Ÿ�̸� ������ ���� �Լ�
void InGameServer::CreateTimer()
{
	m_worker_threads.emplace_back(std::thread(&InGameServer::DoTimer, this, m_hiocp));

}

// ���� ������ �����ϴ� �Լ� ��������, Ÿ�̸Ӿ����� ����, DB������ ����, ������ ���� ����
void InGameServer::Run()
{
	StartServer();
	CreateTimer();
	m_PacketManager->CreateDBThread();
	JoinThread();
	m_PacketManager->JoinDBThread();
}

// ���� ���� �Լ�
void InGameServer::End()
{
	m_PacketManager->End();

	CloseHandle(m_hiocp);
	closesocket(m_s_socket);

}