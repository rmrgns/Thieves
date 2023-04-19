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

	//float	s_x, s_y, s_z;
	//float	f_x, f_y, f_z;
	//float	r_x, r_y, r_z;
//	Vec3 GetForwardVec() { return f_pos; };
//	Vec3 GetRightVec() { return r_pos; };
//	char GetDirection() { return direction; };


	Vec3 GetVec() { return recv_pos; };

	void SetVecX(float p_posx) { recv_pos.x = p_posx; };
	void SetVecY(float p_posy) { recv_pos.y = p_posy; };
	void SetVecZ(float p_posz) { recv_pos.z = p_posz; };
	
private:
	std::unordered_map<int, shared_ptr<NetworkMoveObj>>m_obj_map;
	GameInfo m_game_info;
	//Vec3 f_pos;
	//Vec3 _pos = { 999, 999, 999 };
	Vec3 recv_pos;

	//char direction;
};