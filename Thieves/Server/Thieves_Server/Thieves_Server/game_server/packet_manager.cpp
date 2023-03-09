#include "packet_manager.h"
#include "room/room_manager.h"
#include "../database/db.h"
#include "pch.h"
PacketManager::PacketManager()
{
	m_room_manager = new RoomManager;
	m_db = new DB;
}

void PacketManager::Init()
{
	m_room_manager->InitRoom();

	m_db->Init();
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];

	switch (packet_type) {
	case CS_PACKET_SIGN_IN: {
		ProcessSignIn(c_id, p);
		break;
	}
	case CS_PACKET_SIGN_UP: {
		ProcessSignUp(c_id, p);
		break;
	}
	case CS_PACKET_MOVE: {
		ProcessMove(c_id, p);
		break;
	}
	case CS_PACKET_ATTACK: {
		ProcessAttack(c_id, p);
		break;
	}
	case CS_PACKET_MATCHING: {
		ProcessMatching(c_id, p);
		break;
	}
	case CS_PACKET_HIT: {
		ProcessHit(c_id, p);
		break;
	}
	case CS_PACKET_GAME_START: {
		ProcessGameStart(c_id, p);
		break;
	}
	}
}

void PacketManager::ProcessAccept(HANDLE hiocp, SOCKET& s_socket, EXP_OVER* exp_over)
{
	std::cout << "Accept process" << std::endl;
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->_net_buf));
	int new_id = -1;// = MoveObjManager::GetInst()->GetNewID();
	if (-1 == new_id) {
		std::cout << "Maxmum user overflow. Accept aborted.\n";
		SendLoginFailPacket(c_socket, static_cast<int>(LOGINFAIL_TYPE::FULL));
	}
	else {
		//Player* cl = MoveObjManager::GetInst()->GetPlayer(new_id);
		//cl->SetID(new_id);
		//cl->Init(c_socket);
		//CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), hiocp, new_id, 0);
		//cl->DoRecv();
	}
	ZeroMemory(&exp_over->_wsa_over, sizeof(exp_over->_wsa_over));
	c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	*(reinterpret_cast<SOCKET*>(exp_over->_net_buf)) = c_socket;
	AcceptEx(s_socket, c_socket, exp_over->_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->_wsa_over);
}

void PacketManager::ProcessRecv(int, EXP_OVER*, DWORD)
{
}

void PacketManager::SendMovePacket(int c_id, int mover)
{
}

void PacketManager::SendLoginFailPacket(SOCKET& c_socket, int reason)
{
	sc_packet_login_fail packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_FAIL;
	packet.reason = reason;
	EXP_OVER* ex_over = new EXP_OVER(COMP_OP::OP_SEND, sizeof(packet), &packet);
	int ret = WSASend(c_socket, &ex_over->_wsa_buf, 1, 0, 0, &ex_over->_wsa_over, NULL);
}
void PacketManager::SendSignInOK(int c_id)
{
}

void PacketManager::SendSignUpOK(int c_id)
{
}

void PacketManager::SendPutObjPakcet(int c_id, int obj_id, OBJ_TYPE obj_type)
{
}

void PacketManager::SendTime(int c_id, float round_time)
{
}

void PacketManager::SendAttackPacket(int c_id, int room_id)
{
}

void PacketManager::SendGameWin(int c_id)
{
}

void PacketManager::SendGameDefeat(int c_id)
{
}

void PacketManager::SendStun(int c_id, int obj_id)
{
}

void PacketManager::SendPhasePacket(int c_id, int curr_phase)
{
}

void PacketManager::End()
{
}

void PacketManager::Disconnect(int c_id)
{
}

bool PacketManager::IsRoomInGame(int room_id)
{
	return false;
}

void PacketManager::EndGame(int room_id)
{
}

void PacketManager::CreateDBThread()
{
}

// 비동기로 DB작업을 실행하는 Thread
void PacketManager::DBThread()
{
}

// case에 따른 DB 로그인 가입을 판단하는 함수
void PacketManager::ProcessDBTask(db_task& dt)
{
}

void PacketManager::JoinDBThread()
{
}

void PacketManager::ProcessTimer(HANDLE hiocp)
{
}

void PacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessAttack(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessMove(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessHit(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessGameStart(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessDamageCheat(int c_id, unsigned char* p)
{
}

void PacketManager::StartGame(int room_id)
{
}
