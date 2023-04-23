#pragma once
#include <unordered_map>

#include "server/packet/packet_manager.h"
#include "server/network/network_obj_manager.h"
#include "server/network/network_move_object.h"
#include "thieves_server/game_info.h"
#include "PlayerInput.h"


class ThievesPacketManager : public PacketManager 
{
public:
	ThievesPacketManager() = default;
	virtual ~ThievesPacketManager() = default;

	virtual void Init()override;

	void ProcessTest(int c_id, unsigned char* p);

	void ProcessMove(int c_id, unsigned char* p);
	void ProcessSignin(int c_id, unsigned char* p);
	void ProcessObjInfo(int c_id, unsigned char* p);


	// 추가필요


	void Reset();

	Vec3 GetVec(){ return recv_pos; }

	void SetVecX(float p_posx) { recv_pos.x = p_posx; }
	void SetVecY(float p_posy) { recv_pos.y = p_posy; }
	void SetVecZ(float p_posz) { recv_pos.z = p_posz; }
	
	Vec3 GetPlayerInput() { return recv_pos; }
	bool GetRecv() {return bRecv; }
	void SetRecv(bool bool_recv) { bRecv = bool_recv; }

private:
	std::unordered_map<int, shared_ptr<NetworkMoveObj>>m_obj_map;
	GameInfo m_game_info;

	bool bRecv;
	Vec3 recv_pos;
};