#include "pch.h"
#include <map>
#include <set>
#include <bitset>


#include "packet_manager.h"
#include "room/room_manager.h"
#include "room/room.h"
#include "Lobby.h"
#include "database/db.h"
#include "object/moveobj_manager.h"
#include "object/MapManager.h"
#include "Astar.h"
#include "recast_astar.h"
#include "EVENT.h"
#include "Timer.h"
#include "Item.h"


using namespace std;
PacketManager::PacketManager()
{
	MoveObjManager::GetInst();
	m_Lobby = new Lobby;
	m_room_manager = new RoomManager;
	m_map_manager = new MapManager;
	m_db = new DB;
	m_db2 = new DB;
	m_Timer = new Timer;

}

void PacketManager::Init()
{
	MoveObjManager::GetInst()->InitPlayer();
	MoveObjManager::GetInst()->InitNPC();
	m_Lobby->Init();
	m_room_manager->InitRoom();
	m_map_manager->LoadMap();
	m_map_manager->LoadSpawnArea();
	m_map_manager->LoadEscapePoint();
	m_map_manager->LoadItemSpawnPoint();
	m_map_manager->LoadPlayerSpawnArea();
	m_map_manager->LoadSpecialEscapePoint();
	m_db->Init();
	m_db2->Init();
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];

	switch (packet_type) {
	case CS_PACKET_SIGN_IN: {
		ProcessSignIn(c_id, p);
		cout << "signIN" << endl;
		break;
	}
	case CS_PACKET_SIGN_UP: {
		ProcessSignUp(c_id, p);
		cout << "signup" << endl;
		break;
	}
	case CS_PACKET_MOVE: {
		ProcessMove(c_id, p);
		cout << "move" << endl;
		break;
	}
	case CS_PACKET_ATTACK: {
		ProcessAttack(c_id, p);
		cout << "attack" << endl;
		break;
	}
	case CS_PACKET_MATCHING: {
		ProcessMatching(c_id, p);
		cout << "match" << endl;
		break;
	}
	case CS_PACKET_HIT: {
		ProcessHit(c_id, p);
		cout << "hit" << endl;
		break;
	}
	case CS_PACKET_GAME_START: {
		ProcessGameStart(c_id, p);
		cout << "start" << endl;
		break;
	}
	case CS_PACKET_LOAD_PROGRESSING: {
		ProcessLoadProgressing(c_id, p);
		cout << "progress" << endl;
		break;
	}	
	case CS_PACKET_LOAD_END: {
		ProcessLoadEnd(c_id, p);
		cout << "loadend" << endl;
		break;
	}
	case CS_PACKET_ENTER_ROOM: {
		ProcessEnterRoom(c_id, p);
		cout << "enterroom" << endl;
		break;
	}
	case CS_PACKET_LEAVE_ROOM: {
		ProcessLeaveRoom(c_id, p);
		cout << "leaveroom" << endl;
		break;
	}
	case CS_PACKET_PLAYER_READY: {
		ProcessPlayerReady(c_id, p);
		cout << "ready" << endl;
		break;
	}
	case CS_PACKET_PLAYER_CANCLE_READY: {
		ProcessPlayerCancleReady(c_id, p);
		cout << "cancle" << endl;
		break;
	}
	case CS_PACKET_PLAYER_LOG_OUT: {
		ProcessLogOut(c_id, p);
		cout << "logout" << endl;
		break;
	}
	case CS_PACKET_REQUEST_ROOMS_DATA_FOR_LOBBY: {
		ProcessRoomsDataInLobby(c_id, p);
		cout << "lobbydata" << endl;
		break;
	}
	case CS_PACKET_REQUEST_ROOMS_DATA_FOR_ROOM: {
		ProcessRoomsDataInRoom(c_id, p);
		cout << "roomdata" << endl;
		break;
	}

	case CS_PACKET_BULLET: {
		ProcessBullet(c_id, p);
		cout << "bullet" << endl;
		break;
	}

	case CS_PACKET_STEEL_DIAMOND: {
		ProcessChangePhase(c_id, p);
		cout << "phase" << endl;
		break;
	}
	case CS_PACKET_GET_ITEM: {
		ProcessGetItem(c_id, p);
		cout << "item" << endl;
		break;
	}
	case CS_PACKET_USE_ITEM: {
		ProcessUseItem(c_id, p);
		cout << "item" << endl;
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
		std::cout << packet_size << "byte\n";
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

void PacketManager::ProcessStunEnd(int c_id)
{
	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->SetAttacked(false);

	SendStunEnd(c_id);
}

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
	packet.z = obj->GetPosZ() + 500;
	obj->SetPosZ(packet.z);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cout << "Send OBJ INFO ID : " << c_id << ", x : " << packet.x << ", y : " << packet.y << ", z : " << packet.z << endl;
	cl->DoSend(sizeof(packet), &packet);

}

void PacketManager::SendObjInfoEnd(int c_id)
{
	sc_packet_obj_info_end packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_OBJ_INFO_END;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendTime(int c_id, float round_time)
{
	
}

void PacketManager::SendAttackPacket(int c_id, int room_id)
{
	cs_packet_attack packet;
	packet.type = CS_PACKET_ATTACK;
	packet.size = sizeof(packet);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendGameWin(int c_id)
{
}

void PacketManager::SendGameDefeat(int c_id)
{
}

void PacketManager::SendStun(int c_id, int obj_id)
{
	sc_packet_stun packet;
	packet.type = SC_PACKET_STUN;
	packet.size = sizeof(packet);
	packet.obj_id = obj_id;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendStunEnd(int c_id)
{
	sc_packet_stun_end packet;
	packet.type = SC_PACKET_STUN;
	packet.size = sizeof(packet);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendPhasePacket(int c_id)
{

	sc_packet_phase_change packet;
	packet.type = SC_PACKET_PHASE;
	packet.size = sizeof(packet);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);

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

void PacketManager::SendAllPlayerLoadEnd(int c_id)
{
	sc_packet_all_player_load_end packet;
	packet.type = SC_PACKET_ALL_PLAYER_LOAD_END;
	packet.size = sizeof(packet);
	
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendEnterRoom(int c_id, int p_id)
{
	sc_packet_enter_room packet;
	packet.type = SC_PACKET_ENTER_ROOM;
	packet.size = sizeof(packet);
	packet.id = p_id;
	char* name = MoveObjManager::GetInst()->GetPlayer(p_id)->GetName();

	for (int i = 0; i < MAX_NAME_SIZE; ++i) {
		packet.userName[i] = name[i];
		if (name[i] == '\0') { break; }
	}

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendEnterRoomOk(int c_id, int room_id)
{
	sc_packet_enter_room_ok packet;
	packet.type = SC_PACKET_ENTER_ROOM_OK;
	packet.size = sizeof(packet);
	packet.room_id = room_id;

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

	packet.isReady = m_room_manager->
		GetRoom(MoveObjManager::GetInst()->GetPlayer(p_id)->GetRoomID())->IsPlayerReady(p_id);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendRoomsDataForRoomEnd(int c_id)
{
	sc_packet_rooms_data_for_room_end packet;
	packet.type = SC_PACKET_ROOMS_DATA_FOR_ROOM_END;
	packet.size = sizeof(packet);
	packet.master_id = m_room_manager->
		GetRoom(MoveObjManager::GetInst()->GetPlayer(c_id)->GetRoomID())->GetRoomMasterId();

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendError(int c_id, int err_type, int err_val)
{
	sc_packet_error packet;
	packet.type = SC_PACKET_ERROR;
	packet.size = sizeof(packet);
	packet.err_type = err_type;
	packet.err_val = err_val;

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendGameStart(int c_id)
{
	sc_packet_game_start packet;
	packet.type = SC_PACKET_GAME_START;
	packet.size = sizeof(packet);

	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	cl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendBullet(int c_id, Vector3 collision_point)
{
	sc_packet_bullet packet;
	packet.type = SC_PACKET_BULLET;
	packet.size = sizeof(packet);
	packet.p_x = collision_point.x;
	packet.p_y = collision_point.y;
	packet.p_z = collision_point.z;

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

	if (cl->GetState() == STATE::ST_INGAME ||
		cl->GetState() == STATE::ST_INROOM ||
		cl->GetState() == STATE::ST_INROOMREDDY)
	{
		Room* room = m_room_manager->GetRoom(cl->GetRoomID());
		room->LeaveRoom(c_id);

		sc_packet_remove_object packet;

		packet.size = sizeof(packet);
		packet.type = SC_PACKET_REMOVE_OBJECT;
		packet.id = c_id;

		auto& objs = room->GetObjList();

		for (int obj : objs)
		{
			MoveObjManager::GetInst()->GetPlayer(obj)->DoSend(sizeof(packet), &packet);
		}

	}

	cl->state_lock.lock();
	cl->SetState(STATE::ST_FREE);
	cl->ResetPlayer();
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

void PacketManager::SendItemInfo(int c_id, int item_id)
{
	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	Room* room = m_room_manager->GetRoom(pl->GetRoomID());

	Item* it = room->GetItem(item_id);

	if (it->GetState() == ITEM_STATE::ST_NOTUSED) return;

	sc_packet_item_info packet;
	packet.type = SC_PACKET_ITEM_INFO;
	packet.id = item_id;
	packet.size = sizeof(packet);



	packet.x = it->GetPosX();
	packet.y = it->GetPosY();
	packet.z = it->GetPosZ();

	packet.item_type = it->GetItemCode();

	pl->DoSend(sizeof(packet), &packet);
}

//=====================DB
void PacketManager::CreateDBThread()
{
	db_thread = std::thread([this]() {DBThread(); });
}

void PacketManager::SendLoginFailPacket(int c_id, int reason)
{
	sc_packet_login_fail packet;
	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_LOGIN_FAIL;
	packet.reason = reason;
	pl->DoSend(sizeof(packet), &packet);

}

void PacketManager::SendTimerStart(int c_id, std::chrono::system_clock::time_point tp)
{
	sc_packet_timer_start packet;
	packet.type = SC_PACKET_TIMER_START;
	packet.size = sizeof(packet);
	packet.start_time = tp;
	
	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->DoSend(sizeof(packet), &packet);

}

void PacketManager::SendOpenSafe(int c_id)
{
	sc_packet_open_safe packet;
	packet.type = SC_PACKET_OPEN_SAFE;
	packet.size = sizeof(packet);
	
	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendOpenEscapeArea(int c_id, int r_id)
{
	sc_packet_active_escape packet;
	packet.type = SC_PACKET_ACTIVE_ESCAPE;
	packet.size = sizeof(packet);

	Room* room = m_room_manager->GetRoom(r_id);
	for (int i = 0; i < 3; ++i) {
		packet.x[i] = room->GetEscapePos(i).x;
		packet.y[i] = room->GetEscapePos(i).y;
		packet.z[i] = room->GetEscapePos(i).z;
	}

	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->DoSend(sizeof(packet), &packet);

}

void PacketManager::SendOpenSpecialEscapeArea(int c_id, int r_id)
{
	sc_packet_active_special_escape packet;
	packet.type = SC_PACKET_ACTIVE_SPECIAL_ESCAPE;
	packet.size = sizeof(packet);

	Room* room = m_room_manager->GetRoom(r_id);
	packet.x = room->GetSpecialEscapePos().x;
	packet.y = room->GetSpecialEscapePos().y;
	packet.z = room->GetSpecialEscapePos().z;

	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendInvincible(int c_id, int p_id)
{
	sc_packet_invincible packet;
	packet.type = SC_PACKET_INVINCIBLE_END;
	packet.size = sizeof(packet);
	packet.player = p_id;

	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->DoSend(sizeof(packet), &packet);
}

void PacketManager::SendInvincibleEnd(int c_id, int p_id)
{
	sc_packet_invincible_end packet;
	packet.type = SC_PACKET_INVINCIBLE_END;
	packet.size = sizeof(packet);
	packet.player = p_id;

	Player* pl = MoveObjManager::GetInst()->GetPlayer(c_id);
	pl->DoSend(sizeof(packet), &packet);
}

// �񵿱�� DB�۾��� �����ϴ� Thread
void PacketManager::DBThread()
{
	while (true)
	{
		db_task dt;
		if (!m_db_queue.try_pop(dt))
		{
			this_thread::sleep_for(10ms);
			continue;
		}
		ProcessDBTask(dt);
	}
}

// case�� ���� DB �α��� ������ �Ǵ��ϴ� �Լ�
void PacketManager::ProcessDBTask(db_task& dt)
{
	LOGINFAIL_TYPE ret;//m_db->CheckLoginData(dt.user_id, dt.user_password);
	Player* pl = MoveObjManager::GetInst()->GetPlayer(dt.obj_id);
	switch (dt.dt)
	{
	case DB_TASK_TYPE::SIGN_IN:
	{
		for (int i = 0; i < MAX_USER; ++i)
		{
			Player* other_pl = MoveObjManager::GetInst()->GetPlayer(i);
			other_pl->state_lock.lock();
			if (other_pl->GetState() == STATE::ST_FREE)
			{
				other_pl->state_lock.unlock();
				continue;
			}
			other_pl->state_lock.unlock();
			if (strcmp(other_pl->GetName(), dt.user_id) == 0)
			{
				ret = LOGINFAIL_TYPE::AREADY_SIGHN_IN;
				SendLoginFailPacket(dt.obj_id, (int)ret);
				return;
			}
		}
		ret = m_db->CheckLoginData(dt.user_id, dt.user_password);
		if (ret == LOGINFAIL_TYPE::OK)
		{

			//접속꽉찬거는 accept 쪽에서 보내기주기
			pl->state_lock.lock();
			if (STATE::ST_FREE == pl->GetState() || STATE::ST_ACCEPT == pl->GetState())
			{
				pl->SetState(STATE::ST_LOGIN);
				pl->state_lock.unlock();
			}
			else
				pl->state_lock.unlock();
			strcpy_s(pl->GetName(), MAX_NAME_SIZE, dt.user_id);
			strcpy_s(pl->GetPassword(), MAX_NAME_SIZE, dt.user_password);
			//cout << "이름 : " << pl->GetName() << "비번 : " << pl->GetPassword();
			//여기오면 성공패킷 보내주기
			SendSignInOK(pl->GetID());
		}
		else
		{
			//로그인 실패 패킷보내기
			SendLoginFailPacket(dt.obj_id, static_cast<int>(ret));
		}
		break;
	}
	case DB_TASK_TYPE::SIGN_UP:
	{
		ret = m_db->CheckLoginData(dt.user_id, dt.user_password);
		if (ret == LOGINFAIL_TYPE::NO_ID || ret == LOGINFAIL_TYPE::WRONG_PASSWORD)
		{
			//m_db2->SaveData(dt.user_id, dt.user_password);
			//cout << "들어옴" << endl;
			SendSignUpOK(dt.obj_id);
		}
		else
			// 아이디가 있어 회원가입 불가능 패킷 보내기
			SendLoginFailPacket(dt.obj_id, 6);
		break;
	}
	}
}

void PacketManager::JoinDBThread()
{
	db_thread.join();
}

void PacketManager::ProcessTimer(HANDLE hiocp)
{
	while (true) {
		while (true) {
			m_Timer->timerLock.lock();

			if (m_Timer->timerQ.empty()) {
				m_Timer->timerLock.unlock();
				this_thread::sleep_for(10ms);
				continue;
			}


		auto tEvent = m_Timer->timerQ.top();

		if (tEvent.GetExecTime() > chrono::system_clock::now()) {
			m_Timer->timerLock.unlock();
			this_thread::sleep_for(10ms);
			continue;
		}

		m_Timer->timerQ.pop();

		m_Timer->timerLock.unlock();

		EXP_OVER* exover = new EXP_OVER;

		if (tEvent.GetEventType() == EV_MOVE) {
			exover->_comp_op = COMP_OP::OP_NPC_MOVE;
		}
		else if (tEvent.GetEventType() == EV_STUN_END) {
			exover->_comp_op = COMP_OP::OP_STUN_END;
		}
		else if (tEvent.GetEventType() == EV_TIMER_START) {
			exover->_comp_op = COMP_OP::OP_TIMER_START;
		}
		else if (tEvent.GetEventType() == EV_OPEN_SAFE) {
			exover->_comp_op = COMP_OP::OP_OPEN_SAFE;
		}
		else if (tEvent.GetEventType() == EV_OPEN_ESCAPE_AREA) {
			exover->_comp_op = COMP_OP::OP_OPEN_ESCAPE;
		}
		else if (tEvent.GetEventType() == EV_OPEN_SPECIAL_ESCAPE_AREA) {
			exover->_comp_op = COMP_OP::OP_OPEN_SPECIAL_ESCAPE;
		}
		else if (tEvent.GetEventType() == EV_INVINCIBLE_END) {
			exover->_comp_op = COMP_OP::OP_INVINCIBLE_END;
		}
		else if (tEvent.GetEventType() == EVENT_TYPE::EVENT_NPC_SPAWN) {
			exover->_comp_op = COMP_OP::OP_NPC_SPAWN;
		}
		else if (tEvent.GetEventType() == EVENT_TYPE::EVENT_TIMER_NPC_SPAWN) {
			exover->_comp_op = COMP_OP::OP_NPC_SPAWN;
		}
		else if (tEvent.GetEventType() ==EVENT_TYPE::EVENT_NPC_MOVE) {
			exover->_comp_op = COMP_OP::OP_NPC_MOVE;
		}
		else if (tEvent.GetEventType() == EVENT_TYPE::EVENT_NPC_ATTACK) {
			exover->_comp_op = COMP_OP::OP_NPC_ATTACK;
		}
		else if (tEvent.GetEventType() == EVENT_TYPE::EVENT_TIME) {
			exover->_comp_op = COMP_OP::OP_STUN_END;
		}		
		

		Player* pl = MoveObjManager::GetInst()->GetPlayer(tEvent.GetObjId());
		exover->room_id = pl->GetRoomID();
		PostQueuedCompletionStatus(hiocp, 1, tEvent.GetObjId(), &exover->_wsa_over);
		this_thread::sleep_for(10ms);
		}
	}
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
	//db_task dt;
	//dt.dt = DB_TASK_TYPE::SIGN_UP;
	//dt.obj_id = c_id;
	//strcpy_s(dt.user_id, packet->name);
	//strcpy_s(dt.user_password, packet->password);
	//m_db_queue.push(move(dt));
}

void PacketManager::ProcessAttack(int c_id, unsigned char* p)
{
	cs_packet_attack* packet = reinterpret_cast<cs_packet_attack*>(p);
	
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);

	Vector3 AttackPos = cl->GetPos();
	AttackPos.x += cl->GetRotX() * 10.f;
	AttackPos.z += cl->GetRotZ() * 10.f;

	Room* clRoom = m_room_manager->GetRoom(cl->GetRoomID());

	for (int pl : clRoom->GetObjList()) {
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		Player* cpl = MoveObjManager::GetInst()->GetPlayer(pl);

		SendAttackPacket(pl, clRoom->GetRoomID());
	}

	for (int pl : clRoom->GetObjList()) {
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		Player* cpl = MoveObjManager::GetInst()->GetPlayer(pl);

		if (cpl->GetPos().Dist(cl->GetPos()) > 30.f) {
			Hit(c_id, pl);
		}
	}
}

void PacketManager::Hit(int c_id, int p_id) { //c_id가 p_id를 공격하여 맞추었음.
	Player* pl = MoveObjManager::GetInst()->GetPlayer(p_id);
	pl->SetAttacked(true);
	
	SendStun(p_id, c_id);

	m_Timer->AddTimer(p_id, std::chrono::system_clock::now() + 3s, EVENT_TYPE::EV_STUN_END);
}

void PacketManager::ProcessMove(int c_id, unsigned char* p)
{
	cs_packet_move* packet = reinterpret_cast<cs_packet_move*>(p);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);
	
	if (cl->GetAttacked()) {
		return;
	}

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
			cl->SetUpVelocity(1250.0f);
		}
	}

	cl->SetRotX(packet->vecX);
	cl->SetRotZ(packet->vecZ);
	

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

	//Player
	// collision

	CBox PlayerBox;
	
	Vector3 PlayerRight = Vector3(cl->GetRotX(), 0.f, cl->GetRotZ()).Cross(Vector3(0.0f, 1.0f, 0.0f));

	//center
	PlayerBox.center[0] = cl->GetPosX();
	PlayerBox.center[1] = cl->GetPosY() + 75.f;
	PlayerBox.center[2] = cl->GetPosZ();

	//extent
	PlayerBox.extent[0] = 25.f;
	PlayerBox.extent[1] = 75.f;
	PlayerBox.extent[2] = 25.f;

	//right
	PlayerBox.axis[0][0] = PlayerRight.x;
	PlayerBox.axis[0][1] = PlayerRight.y;
	PlayerBox.axis[0][2] = PlayerRight.z;


	//up
	PlayerBox.axis[1][0] = 0.0f;
	PlayerBox.axis[1][1] = 0.1f;
	PlayerBox.axis[1][2] = 0.0f;

	//look
	PlayerBox.axis[2][0] = cl->GetRotX();
	PlayerBox.axis[2][1] = 0.f;
	PlayerBox.axis[2][2] = cl->GetRotZ();

	//translation
	PlayerBox.translation[0] = cl->GetPosX() - oldPos.x;
	PlayerBox.translation[1] = cl->GetPosY() - oldPos.y;
	PlayerBox.translation[2] = cl->GetPosZ() - oldPos.z;

	cl->SetPos(m_map_manager->checkCollision(PlayerBox, oldPos));

	cl->state_lock.lock();
	if (cl->GetState() != STATE::ST_INGAME)
	{
		cl->state_lock.unlock();
		return;
	}
	else cl->state_lock.unlock();

	Room* room = m_room_manager->GetRoom(cl->GetRoomID());

	if (isnan(cl->GetPosX()) || isnan(cl->GetPosY()) || isnan(cl->GetPosZ())) {
		return;
	}

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
	
	if (player->GetRoomID() == -1) return;


	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	//if (room->IsGameStarted()) return;

	room->SetGameStart();

	//if (room->GetNumberOfPlayer() != room->GetNumberOfReadyPlayer()) {
	//	SendError(c_id, ERROR_PLAYER_NOT_READY, -1);
	//	return;
	//}

	for (int pl : room->GetObjList()) {

		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		Player* cpl = MoveObjManager::GetInst()->GetPlayer(pl);

		/*
		player->state_lock.lock();
		player->SetState(STATE::ST_INGAME);
		player->SetIsActive(true);
		player->state_lock.unlock();
		*/

		cpl->state_lock.lock();
		cpl->SetState(STATE::ST_INGAME);
		cpl->SetIsActive(true);
		cpl->state_lock.unlock();
	}

	StartGame(room->GetRoomID());
}


void PacketManager::ProcessChangePhase(int c_id, unsigned char* p)
{
	cs_packet_steel_diamond* packet = reinterpret_cast<cs_packet_steel_diamond*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	if (player->GetRoomID() == -1) return;
	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	if (room->GetRound() != 0) return;

	for (int pl : room->GetObjList()) {
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		Player* cpl = MoveObjManager::GetInst()->GetPlayer(pl);
		
		SendPhasePacket(pl);
	}
}

void PacketManager::ProcessGetItem(int c_id, unsigned char* p)
{
	cs_packet_get_item* packet = reinterpret_cast<cs_packet_get_item*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);



	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	switch (packet->itemNum) {
	case ITEM_NUM_DIAMOND:
		if (room->GetRound() == 0) {
			// 페이즈 변경
			ChangePhase(c_id);
			player->SetHasDiamond(true);
		}
		break;
	case ITEM_NUM_GUN:
		if (player->GetItem() == -1) {
			player->SetItem(ITEM_NUM_GUN);
			// send 처리
		}
		break;
	case ITEM_NUM_TRAP:
		if (player->GetItem() == -1) {
			player->SetItem(ITEM_NUM_TRAP);
		}
		break;
	case ITEM_NUM_MAP:
		if (player->GetItem() == -1) {
			player->SetItem(ITEM_NUM_MAP);
		}
		break;
	default:
		break;
	}
}

void PacketManager::ProcessUseItem(int c_id, unsigned char* p)
{
	cs_packet_use_item* packet = reinterpret_cast<cs_packet_use_item*>(p);
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	switch (packet->itemNum) {
	case ITEM_NUM_GUN:
		if (player->GetItem() == ITEM_NUM_GUN) {
			player->SetItem(-1);
			// 총 쓴 처리를 해줘야 함
		}
		break;
	case ITEM_NUM_TRAP:
		if (player->GetItem() == ITEM_NUM_TRAP) {
			player->SetItem(-1);
			// 덫을 쓴 처리를 해줘야 함
		}
		break;
	case ITEM_NUM_MAP:
		break;
	default:
		break;
	}
}

void PacketManager::ChangePhase(int c_id)
{
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	if (player->GetRoomID() == -1) return;
	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	if (room->GetRound() != 0) return;

	for (int pl : room->GetObjList()) {
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		Player* cpl = MoveObjManager::GetInst()->GetPlayer(pl);

		SendPhasePacket(pl);
	}
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

	if (player->GetState() == STATE::ST_INGAME)
	{

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

			MoveObjManager::GetInst()->GetPlayer(pl)->state_lock.lock();
			if (MoveObjManager::GetInst()->GetPlayer(pl)->GetLoadProgressed() != 100) {
				MoveObjManager::GetInst()->GetPlayer(pl)->state_lock.unlock();
				return;
			}
			MoveObjManager::GetInst()->GetPlayer(pl)->state_lock.unlock();

		}

	}

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendAllPlayerLoadEnd(pl);
	}

	room->SetRoundStartTime();

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendTimerStart(pl, room->GetRoundStartTime());
	}

	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 5s, EVENT_TYPE::EV_TIMER_START); // objID를 룸 넘버로 사용하면 될 것으로 보임.
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 65s, EVENT_TYPE::EV_OPEN_SAFE);
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 125s, EVENT_TYPE::EV_OPEN_ESCAPE_AREA);
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 185s, EVENT_TYPE::EV_OPEN_SPECIAL_ESCAPE_AREA);
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

		if (room->GetState() == ROOM_STATE::RT_FREE) {
			SendError(c_id, ERROR_ROOM_NOT_EXIST, -1);
			return;
		}

		if (room->GetState() == ROOM_STATE::RT_INGAME) {
			SendError(c_id, ERROR_GAME_IN_PROGRESS, -1);
			return;
		}

		if (room->GetNumberOfPlayer() >= USER_NUM) {
			SendError(c_id, ERROR_ROOM_IS_FULL, -1);
			return;
		}
	}

	player->state_lock.lock();
	player->SetRoomID(room->GetRoomID());
	player->state_lock.unlock();
	room->m_state_lock.lock();
	room->EnterRoom(c_id);
	room->m_state_lock.unlock();

	SendEnterRoomOk(c_id, room->GetRoomID());

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
	player->SetRoomID(-1);
	player->SetState(STATE::ST_LOGIN);
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

		player->state_lock.lock();
		player->SetState(STATE::ST_INROOMREDDY);
		player->state_lock.unlock();
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

		player->state_lock.lock();
		player->SetState(STATE::ST_INROOM);
		player->state_lock.unlock();
	}

	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;
		if (pl == c_id) continue;
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

void PacketManager::ProcessBullet(int c_id, unsigned char* p)
{
	cs_packet_bullet* packet = reinterpret_cast<cs_packet_bullet*>(p);

	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	Vector3 start_pos{ packet->p_x, packet->p_y, packet->p_z};
	Vector3 dir_pos{ packet->d_x*100, packet->d_y*100, packet->d_z*100};
	
	//m_ray_casting->Shoot(bullet  시작 pos, bullet  방향벡터 );
// -> Vector 총알과 충돌하거나 마지막 bulletpoint 좌표 리턴
	Vector3 col_pos = m_ray_casting->Shoot(start_pos, dir_pos);

	SendBullet(c_id, col_pos);
}

void PacketManager::ProcessInvincibleEnd(int c_id)
{
	Player* player = MoveObjManager::GetInst()->GetPlayer(c_id);

	player->SetInvincible(false);

	Room* room = m_room_manager->GetRoom(player->GetRoomID());

	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendInvincibleEnd(pl, c_id);
	}
}

void PacketManager::ProcessOpenSafe(int r_id)
{
	Room* room = m_room_manager->GetRoom(r_id);

	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendOpenSafe(pl);
	}
}

void PacketManager::ProcessOpenEscape(int r_id)
{
	Room* room = m_room_manager->GetRoom(r_id);

	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendOpenEscapeArea(pl, r_id);
	}
}

void PacketManager::ProcessOpenSpecialEscape(int r_id)
{
	Room* room = m_room_manager->GetRoom(r_id);

	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl)) continue;

		SendOpenSpecialEscapeArea(pl, r_id);
	}
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

	std::random_device rd;
	std::default_random_engine dre(rd());
	std::shuffle(m_map_manager->GetPlayerSpawnPos().begin(), m_map_manager->GetPlayerSpawnPos().end(), dre);
	std::shuffle(m_map_manager->GetItemPos().begin(), m_map_manager->GetItemPos().end(), dre);
	std::shuffle(m_map_manager->GetEscapePos().begin(), m_map_manager->GetEscapePos().end(), dre);
	std::shuffle(m_map_manager->GetSpecialEscapePos().begin(), m_map_manager->GetSpecialEscapePos().end(), dre);

	

	for (int i = 0; i < obj_list.size(); ++i)
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(obj_list[i])) continue;

		pl = MoveObjManager::GetInst()->GetPlayer(obj_list[i]);
		pl->SetPos(m_map_manager->GetPlayerSpawnPos().at(i));
		
	}

	for (int i = 0; i < MAX_ITEM; ++i) {
		if (room->GetItem(i) == nullptr) continue;

		Item* it = room->GetItem(i);

		if (i > MAX_ITEM / 2)
		{
			it->SetItemCode(ITEM_NUM_TRAP);
		}
		else
		{
			it->SetItemCode(ITEM_NUM_GUN);
		}

		it->SetState(ITEM_STATE::ST_SPAWNED);
		it->SetPos(m_map_manager->GetItemPos().at(i));
		
	}

	// 탈출 위치

	for (int i = 0; i < 3; ++i)
	{
		room->SetEscapePos(i, m_map_manager->GetEscapePos().at(i));
	}

	// 특별 탈출 위치

	room->SetSpecialEscapePos(m_map_manager->GetSpecialEscapePos().at(0));

	for (auto c_id : room->GetObjList())
	{
		SendGameStart(c_id);
	}

	for (auto c_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(c_id))
			continue;

		pl = MoveObjManager::GetInst()->GetPlayer(c_id);
		SendObjInfo(c_id, c_id);//�ڱ��ڽ�
		for (auto other_id : room->GetObjList())
		{
			if (false == MoveObjManager::GetInst()->IsPlayer(other_id))
				continue;
			if (c_id == other_id)continue;
			SendObjInfo(c_id, other_id);
		}
	}

	for (auto c_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(c_id)) continue;
		for (int i = 0; i < MAX_ITEM; ++i)
		{
			SendItemInfo(c_id, i);
		}
	}

	for (auto c_id : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(c_id))continue;

		pl = MoveObjManager::GetInst()->GetPlayer(c_id);
		SendObjInfoEnd(c_id);
	}
}

////------NPC--------


void PacketManager::SpawnNPC(int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	int curr_round = room->GetRound();
	int NPC_num = 8;


	// 다이아를 훔치면 2라운드 2라운드시 소환시작
	//	if (curr_round == 2) {
	//		for (auto c_id : room->GetObjList())
	//		{
	//			if (false == MoveObjManager::GetInst()->IsPlayer(c_id))
	//				continue;
	//			SendRound(c_id);
	//		}
	//	}


	Enemy* enemy = NULL;
	unordered_set<int>enemy_list;
	for (auto e_id : room->GetObjList())
	{
		if (enemy_list.size() == static_cast<INT64>(NPC_num))
			break;
		if (true == MoveObjManager::GetInst()->IsPlayer(e_id))
			continue;
		Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(e_id);

		if (true == enemy->in_game)
			continue;
		if (enemy->GetType() == OBJ_TYPE::OT_POLICE)
		{
			enemy->in_game = true;
			enemy->SetIsActive(true);
			enemy_list.insert(e_id);
		}
	}
	vector<Vector3>NPC_spawn_area;


	//NPC_spawn_area.push_back(obj);


	int i = 0;
	for (auto& en : enemy_list)
	{
		m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 1s, EVENT_TYPE::EVENT_TIMER_NPC_SPAWN);
	}
	// m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 1s, EVENT_TYPE::EVENT_NPC_SPAWN);
	//m_Timer->AddTimer(room->GetRoomID(), std::chrono::system_clock::now(), EVENT_TYPE::EVENT_NPC_SPAWN);
}

void PacketManager::SpawnNPCTime(int enemy_id, int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = NULL;

	// NPC 스폰좌표 저장
	vector<Vector3>NPC_spawn_area;

	for (auto& obj : m_map_manager->GetNPCSpawnPos()) {
		NPC_spawn_area.push_back(obj);
	}
	enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	enemy->SetSpawnPoint(NPC_spawn_area[enemy_id/8].x, NPC_spawn_area[enemy_id/8].z);
	
	for (auto pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		SendObjInfo(pl, enemy_id);
	}
	m_Timer->AddTimer(enemy_id, std::chrono::system_clock::now(), EVENT_TYPE::EVENT_NPC_MOVE);
}

void PacketManager::DoNpcMove(int enemy_id, int room_id)
{
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	if (false == enemy->GetIsActive())return;

	//NPC POS
	Vector3 npc_pos;

	//이동하려는 pos
	Vector3 target_pos;
	
	
	const Vector3 base_pos; // 경찰 원래 이동하는 pos

	if (true)//적이 없으면 저장된 다음 위치로
	{
		//target_pos = base_pos;
	}
	else
	{
		target_pos = MoveObjManager::GetInst()->GetPlayer(enemy->GetTargetId())->GetPos();
	}
	Vector3 target_vec = Vector3{ target_pos - enemy->GetPos() };
	enemy->DoMove(target_vec);
	

	// A star
	//target_vec으로 이동 필요한것 현재위치와 target 위치
	// 이동방법 각각의 다음 이동 경로 좌표들을 받아와 저장하며 DO MOVE로 NPC 이동시킨다.
	// 이거 고쳐야 돼
	//for (;;)
	//{
	//	if (target_pos == npc_pos)
	//	{
	//		break;
	//	}
	//}
	

	for (auto pl : room->GetObjList())
	{
		//  NPC의 모든 이동을 플레이어에게 전송
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;
		SendMovePacket(pl, enemy_id);
	}
	CallStateMachine(enemy_id, room_id, base_pos);

}


// 1. 적개체 가져옴
// 2. room 객체를 가져옴
// 3. distance_map에 기준 위치로 부터 플레이어 또는 기지까지의 거리와 ID를 저장
// 4. 일정시간마다 주변 플레이어를 체크하고 저의 타겟을 설정
void PacketManager::CallStateMachine(int enemy_id, int room_id, const Vector3& base_pos)
{
	//Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);
	//Room* room = m_room_manager->GetRoom(room_id);

	//Player* player = NULL;
	//auto check_end_time = std::chrono::system_clock::now();
	//auto& check_time = enemy->GetCheckTime();
	//int target_id = enemy->GetTargetId();
	//if (check_time <= check_end_time) {
	//	for (auto pl : room->GetObjList())
	//	{
	//		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;

	//		if (true == MoveObjManager::GetInst()->IsNear(pl, enemy_id))//이거는 시야범위안에 있는지 확인
	//		{
	//			player = MoveObjManager::GetInst()->GetPlayer(pl);
	//			if (false == m_map_manager->CheckInRange2(player->GetPosX(), player->GetPosZ(), enemy->GetPos() + Vector3(-100.0f, 0.0f, -100.0f), enemy->GetPos() + Vector3(100.0f, 0.0f, 100.0f))) continue;
	//			
	//			//플레이어가 가까우면 target_id 플레이어로
	//			target_id = pl;
	//		}
	//	}
	//}
}

void PacketManager::DoEnemyAttack(int enemy_id, int target_id, int room_id)
{
	//초당두발
	Room* room = m_room_manager->GetRoom(room_id);
	Enemy* enemy = MoveObjManager::GetInst()->GetEnemy(enemy_id);

	if (false == enemy->GetIsActive())return;

	if (target_id == BASE_ID)
	{
		Vector3 base_pos;// = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetPos();

		int base_attack_t;

		if (enemy->GetType() == OBJ_TYPE::OT_PLAYER) {
			float dist = enemy->GetPos().Dist(base_pos);
			base_attack_t = (dist / 1500.0f) * 1000;
		}
		//g_timer_queue.push(SetTimerEvent(enemy_id, enemy_id, room_id, EVENT_TYPE::EVENT_NPC_ATTACK, base_attack_t));
	}
	for (int pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(pl))continue;

		SendNPCAttackPacket(pl, enemy_id, target_id);

	}

	auto& attack_time = enemy->GetAttackTime();
	attack_time = chrono::system_clock::now() + 1s;
	const Vector3 base_pos;// = m_map_manager->GetMapObjectByType(OBJ_TYPE::OT_BASE).GetGroundPos();
	//CallStateMachine(enemy_id, room_id, base_pos);
}

void PacketManager::SendNPCAttackPacket(int c_id, int obj_id, int target_id)
{
	sc_packet_attack packet;
	packet.size = sizeof(packet);
	packet.type = SC_PACKET_ATTACK;
	//packet.obj_id = attacker;
	//packet.f_x = forward_vec.x;
	//packet.f_y = forward_vec.y;
	//packet.f_z = forward_vec.z;
	//MoveObjManager::GetInst()->GetPlayer(c_id)->DoSend(sizeof(packet), &packet);
}
