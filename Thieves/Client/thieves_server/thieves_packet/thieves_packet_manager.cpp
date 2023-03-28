#include "pch.h"
<<<<<<< Updated upstream
#include "../Client/server/network_object.h"
//#include "../network_move_object.h"
#include "../thieves_packet/thieves_packet_manager.h"
#include "thieves_packet_manager.h"
#include "../game_info.h"
=======
//#include "server/network_object.h"
//#include "../network_move_object.h"

#include "thieves_packet_manager.h"
#include "server/thieves_server/game_info.h"
>>>>>>> Stashed changes


void ThievesPacketManager::Init()
{
}
<<<<<<< Updated upstream
void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
{

}
void ThievesPacketManager::ProcessSignin(int c_id, unsigned char* p)
{

}
void ThievesPacketManager::ProcessTest(int c_id, unsigned char* p) 
{
	sc_packet_test* packet = reinterpret_cast<sc_packet_test*>(p);
	PacketHelper::
}
=======
//void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
//{
//
//}
//void ThievesPacketManager::ProcessSignin(int c_id, unsigned char* p)
//{
//
//}
//void ThievesPacketManager::ProcessTest(int c_id, unsigned char* p) 
//{
//	sc_packet_test* packet = reinterpret_cast<sc_packet_test*>(p);
//	PacketHelper::
//}
>>>>>>> Stashed changes
