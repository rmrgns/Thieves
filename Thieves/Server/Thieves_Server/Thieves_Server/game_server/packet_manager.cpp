#include <map>
#include <set>
#include <bitset>
#include "pch.h"

#include "packet_manager.h"
#include "room/room_manager.h"
#include "room/room.h"
#include "database/db.h"
#include "object/moveobj_manager.h"
//concurrency::concurrent_priority_queue<timer_event> PacketManager::g_timer_queue = concurrency::concurrent_priority_queue<timer_event>();



PacketManager::PacketManager()
{
	MoveObjManager::GetInst();
	m_room_manager = new RoomManager;
	m_db = new DB;

}

void PacketManager::Init()
{
	MoveObjManager::GetInst()->InitPlayer();
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
	case CS_PACKET_TEST: {
		//ProcessTest(c_id, p);
		break;
	}
	}
}

void PacketManager::ProcessAccept(HANDLE hiocp, SOCKET& s_socket, EXP_OVER* exp_over)
{
	std::cout << "Accept process" << std::endl;
	SOCKET c_socket = *(reinterpret_cast<SOCKET*>(exp_over->_net_buf));

	int new_id = MoveObjManager::GetInst()->GetNewID();
	std::cout << new_id << std::endl;
	if (-1 == new_id) {
		std::cout << "Maxmum user overflow. Accept aborted.\n";
		SendLoginFailPacket(c_socket, static_cast<int>(LOGINFAIL_TYPE::FULL));
	}
	else {
		Player* cl = MoveObjManager::GetInst()->GetPlayer(new_id);
		cl->SetID(new_id);
		cl->Init(c_socket);
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(c_socket), hiocp, new_id, 0);
		cl->DoRecv();
	}
	ZeroMemory(&exp_over->_wsa_over, sizeof(exp_over->_wsa_over));
	c_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);

	*(reinterpret_cast<SOCKET*>(exp_over->_net_buf)) = c_socket;
	AcceptEx(s_socket, c_socket, exp_over->_net_buf + 8, 0, sizeof(SOCKADDR_IN) + 16,
		sizeof(SOCKADDR_IN) + 16, NULL, &exp_over->_wsa_over);
}

void PacketManager::ProcessRecv(int c_id , EXP_OVER* exp_over, DWORD num_bytes)
{
	if (num_bytes == 0) {
		Disconnect(c_id);
		std::cout << "잘못된 정보" << std::endl;
	}
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	int remain_data = num_bytes + cl->m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];
	if (packet_size == 0)std::cout << "packet_size가 0" << cl->GetID();
	while (packet_size <= remain_data) {
		ProcessPacket(c_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}
	if (0 < remain_data) {
		cl->m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
	if (remain_data == 0)cl->m_prev_size = 0;
	cl->DoRecv();
}

//void PacketManager::UpdateObjMove()//일단 보류
//{
//	for (int i = 0; i < MAX_USER; ++i)
//	{
//		for (int j = 0; j <= NPC_ID_END; ++j) {
//			//이후에 조건 추가
//			if (i != j)
//				SendMovePacket(i, j);
//		}
//	}
//	SetTimerEvent(0, 0, EVENT_TYPE::EVENT_PLAYER_MOVE, 10);
//}

// Send test PAcket
void PacketManager::SendTestPacket(int c_id, int mover, float x, float y, float z)
{
	sc_packet_test packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_TEST;
	packet.id = mover;
	packet.x = x;
	packet.y = y;
	packet.z = z;

	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);

}

void PacketManager::SendMovePacket(int c_id, int mover)
{
}

void PacketManager::SendMoveTestPacket(int mover)
{
	sc_packet_move packet;
	MoveObj* p = MoveObjManager::GetInst()->GetMoveObj(mover);
	packet.id = mover;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MOVE;
	packet.direction = p->GetDirection();

	packet.f_x = p->GetPosForwardX();
	packet.f_y = p->GetPosForwardY();
	packet.f_z = p->GetPosForwardZ();

	packet.f_x = p->GetPosRightX();
	packet.f_y = p->GetPosRightY();
	packet.f_z = p->GetPosRightZ();

	packet.move_time = p->m_last_move_time;
	Player* cl = MoveObjManager::GetInst()->GetPlayer(mover);
	cl->DoSend(sizeof(packet), &packet);
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
	MoveObjManager::GetInst()->DestroyObject();
	if (MoveObjManager::GetInst())delete MoveObjManager::GetInst();
	if (m_db)delete m_db;
	//if (m_db2)delete m_db2;
	if (m_room_manager)delete m_room_manager;
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
	std::cout << "MOVE 받음" << std::endl;
	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	
	
	// 
	if (packet->direction == 1)
	{
		packet->f_x = packet->f_x + packet->l_x * _speed * packet->deltaTime;
		packet->f_z = packet->f_z + packet->l_z * _speed * packet->deltaTime;
	}
	if (packet->direction == 2)
	{
		packet->f_x = packet->f_x - packet->l_x * _speed * packet->deltaTime;
		packet->f_z = packet->f_z - packet->l_z * _speed * packet->deltaTime;
	}
	if (packet->direction == 3)
	{
		packet->r_x = packet->r_x - packet->l_x * _speed * packet->deltaTime;
		packet->r_z = packet->r_z - packet->l_z * _speed * packet->deltaTime;
	}
	if (packet->direction == 4)
	{
		packet->r_x = packet->r_x + packet->l_x * _speed * packet->deltaTime;
		packet->r_z = packet->r_z + packet->l_z * _speed * packet->deltaTime;
	}


	//cl->state_lock.lock();
	//if (cl->GetState() != STATE::ST_INGAME)
	//{
	//	cl->state_lock.unlock();
	//	return;
	//}
	//else cl->state_lock.unlock();
	//Room* room = m_room_manager->GetRoom(cl->GetRoomID());

	//cl->m_last_move_time = packet->move_time;

	std::cout << "Packet Forward x :" << packet->f_x << ", y : " << packet->f_y << ", z : " << packet->f_z << std::endl;
	std::cout << "Packet Right   x :" << packet->r_x << ", y : " << packet->r_y << ", z : " << packet->r_z << std::endl;
	//std::cout << "Rotation x :" << packet->r_x << ", y : " << packet->r_y << ", z : " 
	//	<< packet->r_z<< ", w : " << packet->r_w << endl;
	//for (auto other_pl : room->GetObjList())
	//{
	//	if (false == MoveObjManager::GetInst()->IsPlayer(other_pl))
	//		continue;
	//	SendMovePacket(other_pl, c_id);
	//}
	SendMoveTestPacket(c_id);
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
