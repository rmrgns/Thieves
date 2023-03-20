#pragma once
#include <unordered_map>
#include "../packet/packet_manager.h"
#include "../network_obj_manager.h"
#include "../thieves_server/game_info.h"

class ThievesPacketManager : public PacketManager
{
public:
	ThievesPacketManager() = default;
	virtual ~ThievesPacketManager() = default;

	virtual void Init()override;
	
	void ProcessMove(int c_id, unsigned char* p);
	void ProcessSignin(int c_id, unsigned char* p);

private:
	GameInfo m_game_info;
};