#include "PacketManager.hpp"
#include "SendContextManager.h"
#include <span>
#include <iostream>


void PacketManager::Init()
{

}

void PacketManager::ProcessPacket(int sessionId, std::span<char> packet)
{
	if (packet.size() < 2) return;

	unsigned char packetType = packet[1];

	if (handlers[packetType])
	{
		handlers[packetType](sessionId, packet);
	}
	else
	{
		std::cout << "Unknown packet type: " << (int)packetType << "\n";
	}
}

void PacketManager::ProcessSignIn(int sessionId, cs_packet_sign_in* packet)
{
}

void PacketManager::ProcessSignUp(int sessionId, cs_packet_sign_up* packet)
{

}

void PacketManager::ProcessAttack(int sessionId, cs_packet_attack* packet)
{

}

void PacketManager::ProcessMove(int sessionId, cs_packet_move* packet)
{

}

void PacketManager::ProcessHit(int sessionId, cs_packet_hit* packet)
{

}

void PacketManager::ProcessGameStart(int sessionId, cs_packet_game_start* packet)
{

}

void PacketManager::ProcessGetItem(int sessionId, cs_packet_get_item* packet)
{

}

void PacketManager::ProcessUseItem(int sessionId, cs_packet_use_item* packet)
{

}

void PacketManager::ProcessLoadProgressing(int sessionId, cs_packet_load_progressing* packet)
{

}

void PacketManager::ProcessLoadEnd(int sessionId, cs_packet_load_end* packet)
{}

void PacketManager::ProcessEnterRoom(int sessionId, cs_packet_enter_room * packet)
{}

void PacketManager::ProcessLeaveRoom(int sessionId, cs_packet_leave_room * packet)
{}

void PacketManager::ProcessPlayerReady(int sessionId, cs_packet_player_ready * packet)
{}

void PacketManager::ProcessPlayerCancleReady(int sessionId, cs_packet_player_cancle_ready * packet)
{}

void PacketManager::ProcessLogOut(int sessionId, cs_packet_player_log_out * packet)
{}

void PacketManager::ProcessRoomsDataInLobby(int sessionId, cs_packet_request_rooms_data_for_lobby * packet)
{}

void PacketManager::ProcessRoomsDataInRoom(int sessionId, cs_packet_request_rooms_data_for_room * packet)
{}

void PacketManager::ProcessDamageCheat(int sessionId)
{}

void PacketManager::ProcessBullet(int sessionId, cs_packet_bullet * packet)
{}
