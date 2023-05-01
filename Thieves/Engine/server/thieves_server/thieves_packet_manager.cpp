#include "pch.h"

#include "server/main/network.h"
#include "server/main/network_obj_manager.h"
#include "server/main/network_move_object.h"

#include "Engine/server/thieves_server/thieves_packet_manager.h"
#include "server/packet/packet_helper.h"
#include "server/thieves_server/thieves_message_event_info.h"
#include "server/object/player.h"

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

	
	recv_pos = { packet->posX, packet->posY, packet->posZ };
	
	SetVecX(packet->posX);
	SetVecY(packet->posY);
	SetVecZ(packet->posZ);
	SetRecv(packet->recv_bool);
	//PlayerInput::SetJumpState(packet->jump_state);


	// 자기 자신
	//GET_SINGLE(SceneManager)->SetPlayerPosition(recv_pos);
	GET_SINGLE(SceneManager)->SetPlayerPositionX(recv_pos.x);
	//if (packet->jump_state == true)
	//{
		GET_SINGLE(SceneManager)->SetPlayerPositionY(recv_pos.y);
//	}
//	else if(packet->jump_state == false)
//	{
//		GET_SINGLE(SceneManager)->SetPlayerPositionY(0.f);
//	}
	GET_SINGLE(SceneManager)->SetPlayerPositionZ(recv_pos.z);
	
	// other
	//SceneManager::BuildPlayer(float )

	if (mover != m_obj_map.end())
	{
		//if (mover->second->GetIsActive() == false)return;
		if (isnan(packet->posX)|| isnan(packet->posY) ||  isnan(packet->posZ))return;
		
		mover->second->SetPosition(move(recv_pos));
		
	}
}

void ThievesPacketManager::ProcessSignin(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_game_info.SetNetworkID(packet->id);

	// 클라로
}


void ThievesPacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	NetworkObj* obj = NULL;
	Network::matching_end = true;
	NW_OBJ_TYPE nw_type;
	
	packet->id == m_game_info.GetNetworkID() ? nw_type = NW_OBJ_TYPE::OT_MY_PLAYER : nw_type = (NW_OBJ_TYPE)packet->object_type;
	auto res = m_obj_map.try_emplace(packet->id, CreateSPtr<NetworkMoveObj>(
		packet->id,
		nw_type,
		packet->x,
		packet->y,
		packet->z
	));

	// 오브젝트 spqwn 및 m_objmap[pakcet->id] 전송하여 id값마다 스폰
}


//void ThievesPacketManager::Reset()
//{
//	for (auto& obj : m_obj_map)
//	{
//		//if (obj.second->GetIsActive() == true)
//			//PacketHelper::DisconnectActorFromServer(obj.first);
//		obj.second = nullptr;
//
//	}
//	m_obj_map.clear();
//}