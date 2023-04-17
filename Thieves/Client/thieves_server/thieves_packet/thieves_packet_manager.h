#pragma once
#include <unordered_map>

#include "server/packet/packet_manager.h"
#include "server/network/network_obj_manager.h"
#include "server/network/network_move_object.h"
#include "thieves_server/game_info.h"


class ThievesPacketManager : public PacketManager
{
public:
	ThievesPacketManager() = default;
	virtual ~ThievesPacketManager() = default;

	virtual void Init()override;

	void ProcessTest(int c_id, unsigned char* p);

	void ProcessMove(int c_id, unsigned char* p);
	void ProcessSignin(int c_id, unsigned char* p);

	// 추가필요


	void Reset();
	float	s_x, s_y, s_z;
	float	f_x, f_y, f_z;
	float	r_x, r_y, r_z;

private:
	std::unordered_map<int, client_fw::SPtr<NetworkMoveObj>>m_obj_map;
	GameInfo m_game_info;

};