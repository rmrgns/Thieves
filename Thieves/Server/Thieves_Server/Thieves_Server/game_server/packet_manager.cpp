#include <map>
#include <set>
#include <bitset>
#include "pch.h"

#include "packet_manager.h"
#include "room/room_manager.h"
#include "room/room.h"
#include "Lobby.h"
#include "database/db.h"
#include "object/moveobj_manager.h"
#include "object/MapManager.h"
#include "OBB.h"
//concurrency::concurrent_priority_queue<timer_event> PacketManager::g_timer_queue = concurrency::concurrent_priority_queue<timer_event>();

using namespace std;


PacketManager::PacketManager()
{
	MoveObjManager::GetInst();
	m_Lobby = new Lobby;
	m_room_manager = new RoomManager;
	m_map_manager = new MapManager;
	m_db = new DB;

}

void PacketManager::Init()
{
	MoveObjManager::GetInst()->InitPlayer();
	m_Lobby->Init();
	m_room_manager->InitRoom();
	m_map_manager->LoadMap();
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
	case CS_PACKET_LOAD_PROGRESSING: {
		ProcessLoadProgressing(c_id, p);
		break;
	}	
	case CS_PACKET_LOAD_END: {
		ProcessLoadProgressing(c_id, p);
		break;
	}
	case CS_PACKET_ENTER_ROOM: {
		ProcessEnterRoom(c_id, p);
		break;
	}
	case CS_PACKET_LEAVE_ROOM: {
		break;
	}
	case CS_PACKET_PLAYER_READY: {
		break;
	}
	case CS_PACKET_PLAYER_CANCLE_READY: {
		break;
	}
	case CS_PACKET_PLAYER_LOG_OUT: {
		break;
	}
	case CS_PACKET_REQUEST_ROOMS_DATA_FOR_LOBBY: {
		break;
	}
	case CS_PACKET_REQUEST_ROOMS_DATA_FOR_ROOM: {
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
		std::cout << "disconnect" << std::endl;
	}
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	int remain_data = num_bytes + cl->m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];
	if (packet_size == 0) std::cout << "packet_size 0" << cl->GetID();
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

//void PacketManager::UpdateObjMove()
//{
//	for (int i = 0; i < MAX_USER; ++i)
//	{
//		for (int j = 0; j <= NPC_ID_END; ++j) {
//			//���Ŀ� ���� �߰�
//			if (i != j)
//				SendMovePacket(i, j);
//		}
//	}
//	SetTimerEvent(0, 0, EVENT_TYPE::EVENT_PLAYER_MOVE, 10);
//}

//void PacketManager::UpdateObjMove()//�ϴ� ����
//{
//	for (int i = 0; i < MAX_USER; ++i)
//	{
//		for (int j = 0; j <= NPC_ID_END; ++j) {
//			//���Ŀ� ���� �߰�
//			if (i != j)
//				SendMovePacket(i, j);
//		}
//	}
//	SetTimerEvent(0, 0, EVENT_TYPE::EVENT_PLAYER_MOVE, 10);
//}

void PacketManager::SendMovePacket(int c_id, int mover)
{
	sc_packet_move packet;
	MoveObj* p = MoveObjManager::GetInst()->GetMoveObj(mover);
	packet.id = mover;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_MOVE;

	packet.posX = p->GetPosX();
	packet.posY = p->GetPosY();
	packet.posZ = p->GetPosZ();

	packet.rotX = p->GetRotX();
	packet.rotZ = p->GetRotZ();

	packet.recv_bool = true;

	packet.action_type = p->GetAnimationNumber();

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);

	//cout << "ID : " << c_id << " mover " << mover << " x " << packet.posX << "z " << packet.posZ << endl;
	//cout << "ID : " << c_id << " x " << packet.posX  << "z " << packet.posZ << endl;


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
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			std::cout << "Send" << error_num << std::endl;
	}
}
void PacketManager::SendSignInOK(int c_id)
{
	sc_packet_sign_in_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SIGN_IN_OK;
	packet.id = c_id;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendSignUpOK(int c_id)
{
	sc_packet_sign_up_ok packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_SIGN_UP_OK;
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendPutObjPakcet(int c_id, int obj_id, OBJ_TYPE obj_type)
{
	sc_packet_put_object packet;
	packet.id = obj_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_PUT_OBJECT;
	packet.object_type = (char)obj_type;
	packet.x = MoveObjManager::GetInst()->GetMoveObj(obj_id)->GetPosX();
	packet.y = MoveObjManager::GetInst()->GetMoveObj(obj_id)->GetPosY();
	packet.z = MoveObjManager::GetInst()->GetMoveObj(obj_id)->GetPosZ();
	MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendObjInfo(int c_id, int obj_id)
{
	sc_packet_obj_info packet;
	MoveObj* obj = MoveObjManager::GetInst()->GetMoveObj(obj_id);
	packet.id = obj_id;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_OBJ_INFO;

	packet.start = true;

	packet.object_type = static_cast<char>(obj->GetType());

	packet.x = obj->GetPosX();
	packet.y = obj->GetPosY();
	packet.z = obj->GetPosZ();
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cout << "Send OBJ INFO ID : " << c_id << ", x : " << packet.x << ", y : " << packet.y << ", z : " << packet.z << endl;
	cl->DoSend(sizeof(packet), &packet);

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

void PacketManager::SendLoadProgress(int c_id, int p_id, int progressed)
{
	sc_packet_load_progress_percent packet;
	packet.type = SC_PACKET_LOAD_PROGRESS_PERCENT;
	packet.size = sizeof(packet);
	packet.id = p_id;
	packet.percent = progressed;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
	
}

void PacketManager::SendLoadEnd(int c_id, int p_id)
{
	sc_packet_load_end packet;
	packet.type = SC_PACKET_LOAD_END;
	packet.size = sizeof(packet);
	packet.id = p_id;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendEnterRoom(int c_id, int p_id)
{
	sc_packet_enter_room packet;
	packet.type = SC_PACKET_ENTER_ROOM;
	packet.size = sizeof(packet);
	packet.id = p_id;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendLeaveRoom(int c_id, int p_id)
{
	sc_packet_leave_room packet;
	packet.type = SC_PACKET_LEAVE_ROOM;
	packet.size = sizeof(packet);
	packet.id = p_id;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendPlayerReady(int c_id, int p_id)
{
	sc_packet_player_ready packet;
	packet.type = SC_PACKET_PLAYER_READY;
	packet.size = sizeof(packet);
	packet.id = p_id;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendPlayerCancleReady(int c_id, int p_id)
{
	sc_packet_player_cancle_ready packet;
	packet.type = SC_PACKET_PLAYER_CANCLE_READY;
	packet.size = sizeof(packet);
	packet.id = p_id;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendRoomsDataForLobby(int c_id, int room_id)
{
	sc_packet_rooms_data_for_lobby packet;
	packet.type = SC_PACKET_ROOMS_DATA_FOR_LOBBY;
	packet.size = sizeof(packet);
	packet.roomID = room_id;

	Room* room = m_room_manager->GetRoom(room_id);

	packet.playerNum = room->GetNumberOfPlayer();
	packet.isInGame = room->GetState() == ROOM_STATE::RT_INGAME ? true : false;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendRoomsDataForLobbyEnd(int c_id)
{
	sc_packet_rooms_data_for_lobby_end packet;
	packet.type = SC_PACKET_ROOMS_DATA_FOR_LOBBY_END;
	packet.size = sizeof(packet);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendRoomsDataForRoom(int c_id, int p_id)
{
	sc_packet_rooms_data_for_room packet;
	packet.type = SC_PACKET_ROOMS_DATA_FOR_ROOM;
	packet.size = sizeof(packet);
	packet.userId = p_id;
	char* name = MoveObjManager::GetInst()->GetPlayer(p_id)->GetName();

	for (int i = 0; i < MAX_NAME_SIZE; ++i) {
		packet.userName[i] = name[i];
		if (name[i] == '\0') { break; }	
	}

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendRoomsDataForRoomEnd(int c_id)
{
	sc_packet_rooms_data_for_room_end packet;
	packet.type = SC_PACKET_ROOMS_DATA_FOR_ROOM_END;
	packet.size = sizeof(packet);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendError(int c_id, int err_type)
{
	sc_packet_error packet;
	packet.type = SC_PACKET_ERROR;
	packet.size = sizeof(packet);
	packet.err_type = err_type;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
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
	MoveObjManager::GetInst()->Disconnect(c_id);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);

	if (cl->GetState() == STATE::ST_INGAME)
	{
		Room* room = m_room_manager->GetRoom(cl->GetRoomID());
		auto& objs = room->GetObjList();


		if (!objs.empty())
		{
			if (objs.contains(c_id))
			{
				objs.erase(c_id);
			}
		}

		sc_packet_remove_object packet;

		packet.size = sizeof(packet);
		packet.type = SC_PACKET_REMOVE_OBJECT;
		packet.id = c_id;

		for (int obj : objs)
		{
			MoveObjManager::GetInst()->GetPlayer(obj)->DoSend(sizeof(packet), &packet);
		}

	}

	cl->state_lock.lock();
	if (cl->GetRoomID() == 0) {
		cl->SetState(STATE::ST_FREE);
		cl->ResetPlayer();
	}
	cl->state_lock.unlock();
	
	
}

bool PacketManager::IsRoomInGame(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	room->m_state_lock.lock();
	if (room->GetState() == ROOM_STATE::RT_INGAME)
	{
		room->m_state_lock.unlock();
		return true;
	}
	room->m_state_lock.unlock();
	return false;
}

void PacketManager::EndGame(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);

	for (auto obj_id : room->GetObjList())
	{
		MoveObjManager::GetInst()->GetMoveObj(obj_id)->Reset();
	}
}

void PacketManager::CreateDBThread()
{
}

// �񵿱�� DB�۾��� �����ϴ� Thread
void PacketManager::DBThread()
{
}

// case�� ���� DB �α��� ������ �Ǵ��ϴ� �Լ�
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
	cs_packet_sign_in* packet = reinterpret_cast<cs_packet_sign_in*>(p);
	//db_task dt;
	//dt.dt = DB_TASK_TYPE::SIGN_IN;
	//dt.obj_id = c_id;
	//strcpy_s(dt.user_id, packet->name);
	//strcpy_s(dt.user_password, packet->password);
	//m_db_queue.push(move(dt));
	SendSignInOK(c_id);
}

void PacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
	cs_packet_sign_up* packet = reinterpret_cast<cs_packet_sign_up*>(p);
	db_task dt;
	dt.dt = DB_TASK_TYPE::SIGN_UP;
	dt.obj_id = c_id;
	strcpy_s(dt.user_id, packet->name);
	strcpy_s(dt.user_password, packet->password);
	m_db_queue.push(move(dt));
}

void PacketManager::ProcessAttack(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessMove(int c_id, unsigned char* p)
{
	//std::cout << "MOVE " << std::endl;
	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	
	Vector3 oldPos = cl->GetPos();

	if ((packet->direction & 8) == 8)
	{
		// 앞
		cl->SetPosX(cl->GetPosX() + packet->vecX * _speed * packet->deltaTime);
		cl->SetPosZ(cl->GetPosZ() + packet->vecZ * _speed * packet->deltaTime);
	}

	if ((packet->direction & 4) == 4)
	{
		// 왼
		Vector3 look = Vector3(packet->vecX, 0.0f, packet->vecZ);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = look.Cross(up);

		cl->SetPosX(cl->GetPosX() + right.x * _speed * packet->deltaTime);
		cl->SetPosZ(cl->GetPosZ() + right.z * _speed * packet->deltaTime);

	}

	if ((packet->direction & 2) == 2)
	{
		// 뒤
		cl->SetPosX(cl->GetPosX() - packet->vecX * _speed * packet->deltaTime);
		cl->SetPosZ(cl->GetPosZ() - packet->vecZ * _speed * packet->deltaTime);
	}

	if ((packet->direction & 1) == 1)
	{
		// 오
		Vector3 look = Vector3(packet->vecX, 0.0f, packet->vecZ);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 right = look.Cross(up);

		cl->SetPosX(cl->GetPosX() - right.x * _speed * packet->deltaTime);
		cl->SetPosZ(cl->GetPosZ() - right.z * _speed * packet->deltaTime);
	}

	if ((packet->direction & 16) == 16)
	{
		// 점프
		if (!cl->GetJump())
		{
			cl->SetJump(true);
			cl->SetUpVelocity(1000.0f);
		}
	}

	


	cl->SetRotX(packet->vecX);
	cl->SetRotZ(packet->vecZ);
	//cl->SetPosY(packet->vecY);

	Vector3 ObbCenter = cl->GetPos();
	ObbCenter.y += 75.f;

	OBB playerOBB = OBB(ObbCenter,
		Vector3(25.f, 75.f, 25.f),
		Vector3(0.f, 1.f, 0.f),
		Vector3(cl->GetRotX(), 0.f, cl->GetRotZ()).Cross(Vector3(0.0f, 1.0f, 0.0f))
		, Vector3(cl->GetRotX(), 0.f, cl->GetRotZ()));



	if (m_map_manager->isCollision(playerOBB)) cl->SetPos(oldPos);

	cl->SetAnimationNumber(packet->action_type);

	if (cl->GetJump())
	{
		cl->SetUpVelocity(cl->GetUpVelocity() + (packet->deltaTime * -3000.0f));
		cl->SetPosY(cl->GetPosY() + (cl->GetUpVelocity() * packet->deltaTime));

		if (cl->GetPosY() < 0.f)
		{
			cl->SetPosY(0.0f);
			cl->SetJump(false);
		}
	}

	cl->state_lock.lock();
	if (cl->GetState() != STATE::ST_INGAME)
	{
		cl->state_lock.unlock();
		return;
	}
	else cl->state_lock.unlock();
	Room* room = m_room_manager->GetRoom(cl->GetRoomID());

	
	if (isnan(cl->GetPosX()) || isnan(cl->GetPosY()) || isnan(cl->GetPosZ()))return;
	for (auto other_pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(other_pl))
			continue;
		SendMovePacket(other_pl, c_id);
	}	
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessHit(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessGameStart(int c_id, unsigned char* p)
{
	cs_packet_game_start* packet = reinterpret_cast<cs_packet_game_start*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);
	
	player->is_matching = false;
	player->state_lock.lock();
	player->SetState(STATE::ST_INGAME);
	player->SetRoomID(0);
	player->SetIsActive(true);
	player->state_lock.unlock();
	

	if (player->GetRoomID() == -1)return;
	player->SetIsReady(true);
	Room* room = m_room_manager->GetRoom(player->GetRoomID());
	room->EnterRoom(c_id);

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		if (false == MoveObjManager::GetInst()->GetPlayer(pl)->GetIsReady())return;

	}
	StartGame(room->GetRoomID());
}

void PacketManager::ProcessLoadProgressing(int c_id, unsigned char* p)
{
	cs_packet_load_progressing* packet = reinterpret_cast<cs_packet_load_progressing*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	if (player->GetRoomID() == -1) return;
	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	player->state_lock.lock();
	player->SetLoadProgressed(packet->progressed);
	player->state_lock.unlock();

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		if (pl == c_id) continue;


		SendLoadProgress(pl, c_id, packet->progressed);
		
	}

}

void PacketManager::ProcessLoadEnd(int c_id, unsigned char* p)
{
	cs_packet_load_end* packet = reinterpret_cast<cs_packet_load_end*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	if (player->GetRoomID() == -1) return;
	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	player->state_lock.lock();
	player->SetLoadProgressed(100);
	player->state_lock.unlock();

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		if (pl == c_id) continue;

		SendLoadEnd(pl, c_id);
	}

	// 모두 끝났는지 체크해보자

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		if (MoveObjManager::GetInst()->GetPlayer(pl)->GetLoadProgressed() != 100) return;
	}

	// 여기 왔다면 모두 로드가 되었다는 것이다
	// 모두 로드가 되었다면 게임을 시작해주어야 한다.
}

void PacketManager::ProcessEnterRoom(int c_id, unsigned char* p)
{
	cs_packet_enter_room* packet = reinterpret_cast<cs_packet_enter_room*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);


	Room* room;
	if (packet->room_id == -1)
	{
		//GetEmptyRoom 할 때에 상태를 바꿔주는 것을 확인.
		room = m_room_manager->GetRoom(m_room_manager->GetEmptyRoom());
	}
	else
	{
		room = m_room_manager->GetRoom(packet->room_id);

		if (room->GetState() == ROOM_STATE::RT_INGAME) {
			SendError(c_id, ERROR_GAME_IN_PROGRESS);
			return;
		}

		if (room->GetNumberOfPlayer() >= USER_NUM) {
			SendError(c_id, ERROR_ROOM_IS_FULL);
			return;
		}
	}



	player->state_lock.lock();
	player->SetRoomID(room->GetRoomID());
	player->state_lock.unlock();
	room->m_state_lock.lock();
	room->EnterRoom(c_id);
	room->m_state_lock.unlock();

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		if (pl == c_id) continue;
		
		SendEnterRoom(pl, c_id);
	}

}

void PacketManager::ProcessLeaveRoom(int c_id, unsigned char* p)
{
	cs_packet_leave_room* packet = reinterpret_cast<cs_packet_leave_room*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	// 플레이어가 애초에 방에 안들어가 있었다?
	// 뭔가 오류가 있는 거임
	if(player->GetRoomID() == -1) return;


	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	// player가 나가려고 하는데 룸에는 데이터가 없다?? 
	// 뭔가 오류가 있는거임
	// 플레이어는 데이터를 바꿨는데 룸에서는 안바뀐거임.
	if (!room->GetObjList().contains(c_id)) return;

	player->state_lock.lock();
	player->SetRoomID(c_id);
	player->state_lock.unlock();

	room->m_state_lock.lock();
	room->LeaveRoom(c_id);
	room->m_state_lock.unlock();

	if (room->GetState() != ROOM_STATE::RT_FREE)
	{
		for (auto pl : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
			SendLeaveRoom(pl, c_id);
		}
	}
}

void PacketManager::ProcessPlayerReady(int c_id, unsigned char* p)
{
	cs_packet_player_ready* packet = reinterpret_cast<cs_packet_player_ready*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	if (!room->IsPlayerReady(c_id)) {
		room->m_state_lock.lock();
		room->PlayerReady(c_id);
		room->m_state_lock.unlock();
	}

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		SendPlayerReady(pl, c_id);
	}
}

void PacketManager::ProcessPlayerCancleReady(int c_id, unsigned char* p)
{
	cs_packet_player_ready* packet = reinterpret_cast<cs_packet_player_ready*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	if (room->IsPlayerReady(c_id)) {
		room->m_state_lock.lock();
		room->PlayerCancleReady(c_id);
		room->m_state_lock.unlock();
	}

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		SendPlayerCancleReady(pl, c_id);
	}
}

void PacketManager::ProcessLogOut(int c_id, unsigned char* p)
{
	cs_packet_player_ready* packet = reinterpret_cast<cs_packet_player_ready*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	player->state_lock.lock();
	player->SetState(STATE::ST_ACCEPT);
	player->state_lock.unlock();
}

void PacketManager::ProcessRoomsDataInLobby(int c_id, unsigned char* p)
{
	for (Room* room : m_room_manager->GetRooms())
	{
		if (room->GetState() == ROOM_STATE::RT_FREE) continue;
		
		SendRoomsDataForLobby(c_id, room->GetRoomID());
	}

	SendRoomsDataForLobbyEnd(c_id);
}

void PacketManager::ProcessRoomsDataInRoom(int c_id, unsigned char* p)
{
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendRoomsDataForRoom(c_id, pl);
	}

	SendRoomsDataForRoomEnd(c_id);
}

void PacketManager::ProcessDamageCheat(int c_id, unsigned char* p)
{
}

void PacketManager::StartGame(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	//const Vector3 base_pos = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetGroundPos();
	//�� ������Ʈ ������ ������ �ʿ����
	//npc�� player �ʱ�ȭ �� �����ֱ�
	//const Vector3 base_pos = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetGroundPos();

	Player* pl = NULL;
	//Vector3 pos = Vector3(0.0f, 0.0f, 0.0f);
	vector<int>obj_list{ room->GetObjList().begin(),room->GetObjList().end() };

	for (int i = 0; i < obj_list.size(); ++i)
	{
		if (i < room->GetMaxUser())
		{
			pl = MoveObjManager::GetInst()->GetPlayer(obj_list[i]);
			pl->SetPos({ 0.0f,0.0f,0.0f});
			//pl->SetColorType(COLOR_TYPE(i + 1));
			continue;
		}
	}

	for (auto c_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(c_id))
			continue;

		pl = MoveObjManager::GetInst()->GetPlayer(c_id);
		//cout << "SendObj �̸�:" << pl->GetName() << endl;
		SendObjInfo(c_id, c_id);//�ڱ��ڽ�
		for (auto other_id : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(other_id))
				continue;
			if (c_id == other_id)continue;
			SendObjInfo(c_id, other_id);
			//cout << "�ȿ� SendObj �̸�:" << pl->GetName() << endl;
		}
		//SendBaseStatus(c_id, room->GetRoomID());

		//SendWaveInfo(c_id, next_round, room->GetMaxUser() * next_round, room->GetMaxUser() * (next_round + 1));
	}
}

// TEST

// GameStart Test
void PacketManager::TestProcessGameStart(int c_id, unsigned char* p)
{
	cs_packet_game_start* packet = reinterpret_cast<cs_packet_game_start*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);
	player->SetMatchUserSize(USER_NUM);
	player->is_matching = true;
	Player* other_pl = NULL;
	vector<int>match_list;
	match_list.push_back(c_id);
	Room* room = m_room_manager->GetRoom(0);
	for (auto id : match_list)
	{
		Player* pl = MoveObjManager::GetInst()->GetPlayer(id);
		pl->is_matching = false;
		pl->state_lock.lock();
		pl->SetState(STATE::ST_INGAME);
		pl->SetRoomID(0);
		//player->SetIsActive(true);
		pl->state_lock.unlock();
		room->EnterRoom(id);//�濡 ���̵� �Ѱ��ֱ�
		cout << id << endl;
		//SendMatchingOK(id);
	}

	room->Init(player->GetMatchUserSize());
	for (auto obj_id : match_list)
		room->EnterRoom(obj_id);


	if (player->GetRoomID() == -1)return;
	player->SetIsReady(true);
	std::cout << "Room ID : " <<  (m_room_manager->GetRoom(player->GetRoomID())) << std::endl;
	room = m_room_manager->GetRoom(player->GetRoomID());

	TestStartGame(room->GetRoomID());
}

void PacketManager::TestStartGame(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);

	const Vector3 base_pos{ 0.0f,0.0f,0.0f };

	Player* pl = NULL;

	vector<int>obj_list{ room->GetObjList().begin(),room->GetObjList().end() };

	for (int i = 0; i < obj_list.size(); ++i)
	{
		if (i < room->GetMaxUser())
		{
			pl = MoveObjManager::GetInst()->GetPlayer(obj_list[i]);
			pl->SetPos(base_pos);
			//pl->SetColorType(COLOR_TYPE(i + 1));
			continue;
		}
	}



	for (int i = 0; i < obj_list.size(); ++i)
	{
		if (i < room->GetMaxUser())
		{
			pl = MoveObjManager::GetInst()->GetPlayer(obj_list[i]);
			pl ->SetPos(base_pos);
			//pl->SetPos(m_map_manager->PLAYER_SPAWN_POINT[i]);
			//pl->SetColorType(COLOR_TYPE(i + 1));
			continue;
		}
	}

	for (auto c_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(c_id))
			continue;

		pl = MoveObjManager::GetInst()->GetPlayer(c_id);
		//cout << "SendObj �̸�:" << pl->GetName() << endl;
		//SendObjInfo(c_id, c_id);//�ڱ��ڽ�
		for (auto other_id : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(other_id))
				continue;
			if (c_id == other_id)continue;
			SendObjInfo(c_id, other_id);
			//cout << "�ȿ� SendObj �̸�:" << pl->GetName() << endl;
		}
		//SendBaseStatus(c_id, room->GetRoomID());

		//SendWaveInfo(c_id, next_round, room->GetMaxUser() * next_round, room->GetMaxUser() * (next_round + 1));
	}
}

