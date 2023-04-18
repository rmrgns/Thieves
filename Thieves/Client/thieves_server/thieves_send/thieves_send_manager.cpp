
#include "pch.h"

#include "thieves_server/thieves_send/thieves_send_manager.h"
#include "thieves_server/thieves_message/thieves_message_event_info.h"
#include <server/network/network.h>
#include "server/send_manager/send_manager.h"

//void ThievesSendManager::ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message)
//{
//	//switch (message->GetEventID())
//	//{
//	//case HashCode("send test"): {
//	//	auto msg = std::static_pointer_cast<thieves::TestMessageEventInfo>(message);
//	//	//
//	//	SendTestPacket(s_socket);
//
//	//}
//	//case HashCode("send sign in"): {
//	//	auto msg = std::static_pointer_cast<thieves::SignInMessageEventInfo>(message);
//	//	//LOG_INFO("send sign in");
//	//	SendSignInPacket(s_socket, msg->GetUserID(), msg->GetUserPassword());
//	//	break;
//	//}
//	//case HashCode("send sign up"): {
//	//	auto msg = std::static_pointer_cast<revive::SignUpMessageEventInfo>(message);
//	//	//LOG_INFO("send sign up");
//	//	SendSignUPPacket(s_socket, msg->GetUserID(), msg->GetUserPassword());
//	//	break;
//	//}
//	//case HashCode("send sign matching"): {
//	//	auto msg = std::static_pointer_cast<revive::MatchingMessageEventInfo>(message);
//	//	//LOG_INFO("send sign matching");
//	//	SendMatchingPacket(s_socket, msg->GetUserNum());
//	//	break;
//	//}
//	//case HashCode("send attack"): {
//	//	auto msg = std::static_pointer_cast<revive::SendAttackEventInfo>(message);
//	//	//LOG_INFO("send attack");
//	//	SendAttackPacket(s_socket, msg->GetForward());
//	//	break;
//	//}
//	//case HashCode("send hit"): {
//	//	auto msg = std::static_pointer_cast<revive::ObjectHitMessageEventInfo>(message);
//	//	//LOG_INFO("send hit");
//	//	SendHitPacket(s_socket, msg->GetVictimID(), msg->GetAttackerID());
//	//	break;
//	//}
//	//case HashCode("game start"): {
//	//	auto msg = std::static_pointer_cast<revive::GameStartEventInfo>(message);
//	//	//LOG_INFO("game start");
//	//	SendGameStartPacket(s_socket);
//	//	break;
//	//}
//	//case HashCode("send damage cheat"): {
//	//	auto msg = std::static_pointer_cast<revive::SendDamageCheatEventInfo>(message);
//	//	SendDamageCheatPacket(s_socket);
//	//	break;
//	//}
//	}
//}
//
////void ThievesSendManager::SendTestPacket(const SOCKET& s_socket)
////{
////	cs_packet_test packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_TEST;
////
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
//
//void ThievesSendManager::SendMovePacket(const SOCKET& s_socket, char direction,
//	float f_x, float f_y, float f_z,
//	float r_x, float r_y, float r_z,
//	float deltatime)
//{
//	cs_packet_move packet;
//	packet.size = sizeof(packet);
//	packet.type = CS_PACKET_MOVE;
//
//	packet.f_x = f_x;
//	packet.f_y = f_y;
//	packet.f_z = f_z;
//	packet.r_x = r_x;
//	packet.r_y = r_y;
//	packet.r_z = r_z;
//
//	packet.deltaTime = deltatime;
//	packet.direction = direction;
//
//	SendPacket(s_socket, sizeof(packet), &packet);
//}

void ThievesSendManager::SendMovePacket(const SOCKET& s_socket, char direction,
	Vec3 pos, Vec3 vec, float deltatime)
{
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;

	packet.posX = pos.x;
	packet.posY = pos.y;
	packet.posZ = pos.z;

	packet.vecX = vec.x;
	packet.vecY = vec.y;
	packet.vecZ = vec.z;

	packet.deltaTime = deltatime;
	packet.direction = direction;

	SendPacket(s_socket, sizeof(packet), &packet);
}


//
////void ThievesSendManager::SendSignInPacket(const SOCKET& s_socket, char* id, char* pw)
////{ 
////	cs_packet_sign_in packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_SIGN_IN;
////	strcpy_s(packet.name, id);
////	strcpy_s(packet.password, pw);
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendSignUPPacket(const SOCKET& s_socket, char* id, char* pw)
////{
////	cs_packet_sign_up packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_SIGN_UP;
////	strcpy_s(packet.name, id);
////	strcpy_s(packet.password, pw);
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendMatchingPacket(const SOCKET& s_socket, int user_num)
////{
////	cs_packet_matching packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_MATCHING;
////	packet.user_num = user_num;
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendMovePacket(const SOCKET& s_socket, client_fw::Vec3& pos, client_fw::Quaternion& rot)
////{
////	cs_packet_move packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_MOVE;
////	packet.x = pos.x;
////	packet.y = pos.y;
////	packet.z = pos.z;
////
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendAttackPacket(const SOCKET& s_socket, const client_fw::Vec3& forward_vec)
////{
////	cs_packet_attack packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_ATTACK;
////
////	packet.f_x = forward_vec.x;
////	packet.f_y = forward_vec.y;
////	packet.f_z = forward_vec.z;
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendHitPacket(const SOCKET& s_socket, int obj_id, int attacker_id)
////{
////	cs_packet_hit packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_HIT;
////	packet.victim_id = obj_id;
////	packet.attacker_id = attacker_id;
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendGameStartPacket(const SOCKET& s_socket)
////{
////	cs_packet_game_start packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_GAME_START;
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
////
////void ThievesSendManager::SendDamageCheatPacket(const SOCKET& s_socket)
////{
////	cs_packet_damage_cheat packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_DAMAGE_CHEAT;
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
