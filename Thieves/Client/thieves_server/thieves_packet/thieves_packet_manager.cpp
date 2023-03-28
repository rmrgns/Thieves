#include "pch.h"

#include "server/network/network.h"
#include "server/network/network_obj_manager.h"
#include "server/network/network_move_object.h"

#include "thieves_packet_manager.h"
#include "server/packet/packet_helper.h"
#include "thieves_server/thieves_message/thieves_message_event_info.h"

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
}

void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
{

}
void ThievesPacketManager::ProcessSignin(int c_id, unsigned char* p)
{

}

