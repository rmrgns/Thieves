#include "thieves_send_manager.h"

#include "pch.h"

#include "server/thieves_server/thieves_send_manager.h"
#include "server/thieves_server/thieves_message_event_info.h"
#include <server/main/network.h>
#include "server/main//send_manager.h"

void ThievesSendManager::SendStartPacket(const SOCKET& s_socket)
{
	cs_packet_game_start packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_GAME_START;
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendSignInPacket(const SOCKET& s_socket)
{
	cs_packet_sign_in packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_SIGN_IN;

	SendPacket(s_socket, sizeof(packet), &packet);

}

void ThievesSendManager::SendMovePacket(const SOCKET& s_socket, char direction,
	Vec3 pos, Vec3 vec, float deltatime, char action_type)
{
	cs_packet_move packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_MOVE;

	packet.vecX = vec.x;
	packet.vecZ = vec.z;

	packet.deltaTime = deltatime;
	packet.direction = direction;

	packet.action_type = (char)action_type;

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendLoadProgressPacket(const SOCKET& s_socket,	char progressed)
{
	cs_packet_load_progressing packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_LOAD_PROGRESSING;

	packet.progressed = progressed;


	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendLoadEnd(const SOCKET& s_socket)
{
	cs_packet_load_end packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_LOAD_END;

	SendPacket(s_socket, sizeof(packet), &packet);
	
}

void ThievesSendManager::SendAttackPacket(const SOCKET& s_socket, const Vec3& forward_vec
	, char action_type)
{
	cs_packet_attack packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_ATTACK;
	// �̰� ��ü ����?
	packet.type = (char)action_type;
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendEnterRoom(const SOCKET& s_socket, int room_id)
{
	cs_packet_enter_room packet;
	packet.type = CS_PACKET_ENTER_ROOM;
	packet.size = sizeof(packet);

	packet.room_id = room_id;
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendLeaveRoom(const SOCKET& s_socket)
{
	cs_packet_leave_room packet;
	packet.type = CS_PACKET_LEAVE_ROOM;
	packet.size = sizeof(packet);

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendReady(const SOCKET& s_socket)
{
	cs_packet_player_ready packet;
	packet.type = CS_PACKET_PLAYER_READY;
	packet.size = sizeof(packet);

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendCancleReady(const SOCKET& s_socket)
{
	cs_packet_player_cancle_ready packet;
	packet.type = CS_PACKET_PLAYER_CANCLE_READY;
	packet.size = sizeof(packet);

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendLogOut(const SOCKET& s_socket)
{
	cs_packet_player_log_out packet;
	packet.type = CS_PACKET_PLAYER_LOG_OUT;
	packet.size = sizeof(packet);

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendRequestRoomsData(const SOCKET& s_socket)
{
	cs_packet_request_rooms_data_for_lobby packet;
	packet.type = CS_PACKET_REQUEST_ROOMS_DATA_FOR_LOBBY;
	packet.size = sizeof(packet);

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendRequestInRoomData(const SOCKET& s_socket)
{
	cs_packet_request_rooms_data_for_room packet;
	packet.type = CS_PACKET_REQUEST_ROOMS_DATA_FOR_ROOM;
	packet.size = sizeof(packet);

	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendBullet(const SOCKET& s_socket, Vec3 start_pos, Vec3 dir_pos, Vec3 rot_pos)
{
	cs_packet_bullet packet;

	packet.type = CS_PACKET_BULLET;
	packet.p_x = start_pos.x;
	packet.p_y = start_pos.y;
	packet.p_z = start_pos.z;

	packet.d_x = dir_pos.x;
	packet.d_y = dir_pos.y;
	packet.d_z = dir_pos.z;

	packet.r_x = rot_pos.x;
	packet.r_y = rot_pos.y;
	packet.r_z = rot_pos.z;
	packet.size = sizeof(packet);
	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendAattackMode(const SOCKET& s_socket, char mode)
{
	
}



//void ThievesSendManager::ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<MessageEventInfo>& message)
//{
//	switch (message->GetEventID())
//	{
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

////
////void ThievesSendManager::SendDamageCheatPacket(const SOCKET& s_socket)
////{
////	cs_packet_damage_cheat packet;
////	packet.size = sizeof(packet);
////	packet.type = CS_PACKET_DAMAGE_CHEAT;
////	SendPacket(s_socket, sizeof(packet), &packet);
////}
