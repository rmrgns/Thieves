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
using namespace std;
using namespace client_fw;

void ThievesPacketManager::Init()
{
	//m_obj_map = unordered_map<int, client_fw::SPtr<NetworkMoveObj>>();
	//TEST
	RegisterRecvFunction(SC_PACKET_TEST, [this](int c_id, unsigned char* p) {ProcessTest(c_id, p); });
}

void ThievesPacketManager::ProcessTest(int c_id, unsigned char* p)
{
	sc_packet_test* packet = reinterpret_cast<sc_packet_test*>(p);
	
	
	//PacketHelper::RegisterPacketEventToActor(CreateSPtr<TestMessageEventInfo>())
	//PacketHelper::RegisterPacketEventToActor(CreateSPtr<thieves::TestMessageEventInfo>(HashCode("testmove"),  packet->x, packet->y, packet->z), packet->id);
	//Vec3 recv_pos{ packet->x,packet->y,packet->z };


}

void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);

	pos.x = packet->posX;
	pos.y = packet->posY;
	pos.z = packet->posZ;

	//Vec3 f_pos;
	//Vec3 r_pos;

	/*f_pos.x = packet->f_x;
	f_pos.z = packet->f_z;
	r_pos.x = packet->r_x;
	r_pos.z = packet->f_y;*/



//	direction = packet->direction;

	//f_z = packet->f_z;
	//r_x = packet->r_x;
	//r_y = packet->r_y;
	//r_z = packet->r_z;
	// 
	// 
	// 
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

}

