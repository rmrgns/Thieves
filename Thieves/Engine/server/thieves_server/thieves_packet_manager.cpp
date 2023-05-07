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
#include "Scene.h"
#include "GameObject.h"
#include "NetworkSystem.h"

using namespace std;
using namespace client_fw;

void ThievesPacketManager::Init()
{
	m_obj_map = unordered_map<int, shared_ptr<NetworkMoveObj>>();
	
	RegisterRecvFunction(SC_PACKET_MOVE, [this](int c_id, unsigned char* p) {ProcessMove(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OBJ_INFO, [this](int c_id, unsigned char* p) {ProcessObjInfo(c_id, p); });
	RegisterRecvFunction(SC_PACKET_SIGN_IN_OK, [this](int c_id, unsigned char* p) {ProcessSignin(c_id, p); });
	RegisterRecvFunction(SC_PACKET_REMOVE_OBJECT, [this](int c_id, unsigned char* p) {ProcessRemoveObj(c_id, p); });


}

void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
	int mid = packet->id;
	auto mover = m_obj_map.find(packet->id);

	if (mover != m_obj_map.end())
	{
		//if (mover->second->GetIsActive() == false)return;
		if (isnan(packet->posX) ||  isnan(packet->posZ)) return;
		
		
		mover->second->SetPosition(Vec3(packet->posX, packet->posY, packet->posZ));
		
		//가져온 rot 값은 look vector 이므로 이 부분을 Rotation 각도 값으로 변경해 주어야 함.
		mover->second->SetRotation(Vec3(0.0f, atan2(packet->rotX, packet->rotZ), 0.0f));
		mover->second->SetActionType(packet->action_type);
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
	//NetworkObj* obj = NULL;
	Network::matching_end = true;
	NW_OBJ_TYPE nw_type;
	
	// 이거 가독성 너무 안좋다. 이런 방식으로 적어주면 좋을듯 -> 김혁동임
	// 
	// 패킷 아이디가 게임 정보의 네트워크 아이디와 같다면 오브젝트 타입을 조작 플레이어 타입으로, 아니라면 패킷에서 받은 오브젝트 타입으로 한다.
	packet->id == m_game_info.GetNetworkID() ? nw_type = NW_OBJ_TYPE::OT_MY_PLAYER : nw_type = NW_OBJ_TYPE::OT_PLAYER;
	//-> 나중에 NPC 처리를 할 때에는 이 부분을 변경해 주어야 함.


	// 해당 데이터를 네트워크의 id를 키로 해서 네트워크 오브젝트에 넣는다.
	auto res = m_obj_map.try_emplace(packet->id, CreateSPtr<NetworkMoveObj>(
		packet->id,
		nw_type,
		packet->x,
		packet->y,
		packet->z
	));


	// 만약 이미 게임이 진행되고 있는 상태라면 새롭게 지정해 주어야 함.
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		if (m_game_info.GetNetworkID() == packet->id) return;
		if (m_obj_map.contains(packet->id)) return;

		auto& objects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();


		for (auto& obj : objects)
		{
			auto nsys = obj->GetNetworkSystem();

			if (nsys)
			{
				if (nsys->GetNetworkingType() == NetworkType::NONE)
				{

					nsys->SetNetworkId(packet->id);

					switch (nw_type)
					{
						case NW_OBJ_TYPE::OT_PLAYER:
						{
							nsys->SetNetworkingType(NetworkType::OTHER_PLAYER);
						}
						break;
						// 현재 OT_PLAYER 이외에는 들어와서는 안됨
						case NW_OBJ_TYPE::OT_MY_PLAYER:
						case NW_OBJ_TYPE::OT_NONE:
						default:
							break;
					}

					break;

				}
			}

		}
	}

	// 오브젝트 spqwn 및 m_objmap[pakcet->id] 전송하여 id값마다 스폰
}

void ThievesPacketManager::ProcessRemoveObj(int c_id, unsigned char* p)
{
	sc_packet_remove_object* packet = reinterpret_cast<sc_packet_remove_object*>(p);

	m_obj_map.erase(packet->id);

	auto& objects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& obj : objects) {

		auto nsys = obj->GetNetworkSystem();

		if (nsys)
		{
			if ((nsys->GetNetworkingType() != NetworkType::NONE) && (nsys->GetNetworkId() == packet->id))
			{
				nsys->SetNetworkId(-1);
				nsys->SetNetworkingType(NetworkType::NONE);
				nsys->GetTransform()->SetLocalPosition(Vec3(0.0f, -300.0f, 0.0f));
			}
		}
	}

}


void ThievesPacketManager::Reset()
{
	//for (auto& obj : m_obj_map)
	//{
	//	if (obj.second->GetIsActive() == true) {
	//		if (m_connected_actor_map.find(id) != m_connected_actor_map.cend()
	//					&& m_connected_actor_map[id] != nullptr)
	//				{
	//					SPtr<Actor> actor = m_connected_actor_map[id];
	//					actor->ConnectServer(false);
	//					m_connected_actor_map[id] = nullptr;
	//					return actor;
	//				}
	//				else
	//				{
	//					//LOG_WARN("ID : {0} is already disconnected", id);
	//					return nullptr;
	//				}
	//	}
	//		
	//	obj.second = nullptr;

	//}
	//m_obj_map.clear();
}