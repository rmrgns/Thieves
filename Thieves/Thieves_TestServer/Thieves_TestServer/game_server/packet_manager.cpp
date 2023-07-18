#include <map>
#include <set>
#include <bitset>
#include "pch.h"

#include "packet_manager.h"
#include "room/room_manager.h"
#include "room/room.h"
#include "database/db.h"
#include "object/moveobj_manager.h"
#include "object/MapManager.h"
#include "ray_casting.h"

using namespace std;

PacketManager::PacketManager()
{
	MoveObjManager::GetInst();
	m_room_manager = new RoomManager;
	m_map_manager = new MapManager;
	m_db = new DB;

}

void PacketManager::Init()
{
	MoveObjManager::GetInst()->InitPlayer();
	m_room_manager->InitRoom();
	m_map_manager->LoadMap();
	m_map_manager->LoadEscapeArea();
	m_map_manager->LoadSpecialEscapeArea();
	m_map_manager->LoadItem();
	m_map_manager->LoadSpawnArea();
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

void PacketManager::SendBullet(int c_id, Vector3)
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
	MoveObjManager::GetInst()->Disconnect(c_id);
	Player* cl = MoveObjManager::GetInst()->GetPlayer(c_id);

	if (cl->GetState() == STATE::ST_INGAME)
	{
		Room* room = m_room_manager->GetRoom(cl->GetRoomID());
		auto& objs = room->GetObjList();


		if (!objs.empty())
		{
			if (find(objs.begin(), objs.end(), c_id) != objs.end())
			{
				objs.erase(remove(objs.begin(), objs.end(), c_id));
			}
		}

		sc_packet_remove_object packet;

		packet.size = sizeof(packet);
		packet.type = SC_PACKET_REMOVE_OBJECT;
		packet.id = c_id;

		for (int& obj : objs)
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


	if (isnan(cl->GetPosX()) || isnan(cl->GetPosY()) || isnan(cl->GetPosZ()))return;
	for (auto other_pl : room->GetObjList())
	{
		if (false == MoveObjManager::GetInst()->IsPlayer(other_pl))
			continue;
		SendMovePacket(other_pl, c_id);
	}
}

void PacketManager::ProcessBullet(int c_id, unsigned char* p)
{
	cs_packet_bullet* packet = reinterpret_cast<cs_packet_bullet*>(p);

	//m_ray_casting->Shoot(bullet  시작 pos, bullet  방향벡터 );
	// -> Vector 총알과 충돌하거나 마지막 bulletpoint 좌표 리턴
	// 
	//SendBullet(int, Vector3);
	
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
	packet->progressed;

	

}

void PacketManager::ProcessLoadEnd(int c_id, unsigned char* p)
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
			pl->SetPos({ 200.0f,0.0f,200.0f});
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