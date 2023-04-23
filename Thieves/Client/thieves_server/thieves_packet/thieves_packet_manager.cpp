#include "pch.h"

#include "server/network/network.h"
#include "server/network/network_obj_manager.h"
#include "server/network/network_move_object.h"

#include "thieves_packet_manager.h"
#include "server/packet/packet_helper.h"
#include "thieves_server/thieves_message/thieves_message_event_info.h"
#include "player.h"

#include "PlayerInput.h"
#include "Transform.h"
#include "Engine.h"
#include "Input.h"
#include "SceneManager.h"

using namespace std;
using namespace client_fw;

void ThievesPacketManager::Init()
{
	m_obj_map = unordered_map<int, shared_ptr<NetworkMoveObj>>();

	RegisterRecvFunction(SC_PACKET_MOVE, [this](int c_id, unsigned char* p) {ProcessMove(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OBJ_INFO, [this](int c_id, unsigned char* p) {ProcessObjInfo(c_id, p); });

}

void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);

	auto mover = m_obj_map.find(packet->id);

	
	recv_pos = { packet->posX,packet->posY,packet->posZ };
	
	SetVecX(packet->posX);
	SetVecY(packet->posY);
	SetVecZ(packet->posZ);
	SetRecv(packet->recv_bool);

	GET_SINGLE(SceneManager)->SetPlayerPosition(recv_pos);

	if (mover != m_obj_map.end())
	{
		//if (mover->second->GetIsActive() == false)return;
		if (isnan(packet->posX) || isnan(packet->posY) || isnan(packet->posZ))return;
		//auto end_t = std::chrono::system_clock::now();
		//PacketHelper::RegisterPacketEventToActor(std::make_shared<MoveObjectMessageEventInfo>(HashCode("move object"), mover->second->GetPosition()), packet->id);

		mover->second->SetPosition(move(recv_pos));
		
		GET_SINGLE(SceneManager)->SetPlayerPosition(recv_pos);
		
		//PlayerInput::PlayerMove(recv_pos);
	}

	
	//_pos.x = packet->posX;
	//_pos.y = packet->posY;
	//_pos.z = packet->posZ;
	//
	//SetVecX(packet->posX);
	//SetVecY(packet->posY);
	//SetVecZ(packet->posZ);


	//Vec3 f_pos;
	//Vec3 r_pos;
	/*f_pos.x = packet->f_x;
	//f_pos.z = packet->f_z;
	r_pos.x = packet->r_x;
	r_pos.z = packet->f_y;*/
//	direction = packet->direction;
	//f_z = packet->f_z;
	//r_x = packet->r_x;
	//r_y = packet->r_y;
	//r_z = packet->r_z;
 
	//auto mover = m_obj_map.find(packet->id);
	//if (mover != m_obj_map.end())
	//{
	//	if (mover->second->GetIsActive() == false)return;
	//	if (isnan(packet->x) || isnan(packet->y) || isnan(packet->z))return;
	//	//auto end_t = std::chrono::system_clock::now();


	//	mover->second->SetPosition(move(recv_pos));

	//	//	if (mover->second->m_move_time <= end_t) {
	//	PacketHelper::RegisterPacketEventToActor(CreateSPtr<revive::MoveObjectMessageEventInfo>(HashCode("move object"), mover->second->GetPosition()), packet->id);
	//	//	mover->second->m_move_time = end_t + 50ms;
	////	}
	//}

}

void ThievesPacketManager::ProcessSignin(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_game_info.SetNetworkID(packet->id);

	// Å¬¶ó·Î
}


void ThievesPacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	NetworkObj* obj = NULL;
	Network::matching_end = true;
	NW_OBJ_TYPE nw_type;
	//string da = "ss";
	packet->id == m_game_info.GetNetworkID() ? nw_type = NW_OBJ_TYPE::OT_MY_PLAYER : nw_type = (NW_OBJ_TYPE)packet->object_type;
	auto res = m_obj_map.try_emplace(packet->id, CreateSPtr<NetworkMoveObj>(
		packet->id,
		nw_type,
		packet->x,
		packet->y,
		packet->z
	));
	//PacketHelper::RegisterPacketEventToLevel(CreateSPtr<revive::ObjectInfoMessageEventInfo>(HashCode("spawn object"), m_obj_map[packet->id]));
}
