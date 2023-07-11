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

void ThievesSendManager::SendLoadProgressPacket(const SOCKET& s_socket, char direction,
	char progressed)
{
	cs_packet_load_progressing packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_LOAD_PROGRESSING;

	packet.progressed = progressed;


	SendPacket(s_socket, sizeof(packet), &packet);
}

void ThievesSendManager::SendAttackPacket(const SOCKET& s_socket, const Vec3& forward_vec
	, char action_type)
{
	cs_packet_attack packet;
	packet.size = sizeof(packet);
	packet.type = CS_PACKET_ATTACK;

	packet.type = (char)action_type;
	SendPacket(s_socket, sizeof(packet), &packet);
}

