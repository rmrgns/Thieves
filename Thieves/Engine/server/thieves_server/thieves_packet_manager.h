#pragma once
#include <unordered_map>

#include "server/main/packet_manager.h"
#include "server/main/network_obj_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"
#include "PlayerInput.h"


class ThievesPacketManager : public PacketManager 
{
public:
	ThievesPacketManager() = default;
	virtual ~ThievesPacketManager() = default;

	virtual void Init()override;
	// MOVE
	void ProcessMove(int c_id, unsigned char* p);
	// Login
	void ProcessSignin(int c_id, unsigned char* p);
	// Object����
	void ProcessObjInfo(int c_id, unsigned char* p);
	// JumpStart
	void ProcessJumpStart(int c_id, unsigned char* p);
	// JumpComplete
	void ProcessJumpComplete(int c_id, unsigned char* p);


	// �߰��ʿ�
	void Reset();

	Vec3 GetVec(){ return recv_pos; }

	void SetVecX(float& p_posx) { recv_pos.x = p_posx; }
	void SetVecY(float& p_posy) { recv_pos.y = p_posy; }
	void SetVecZ(float& p_posz) { recv_pos.z = p_posz; }
	
	Vec3 GetPlayerInput() { return recv_pos; }
	bool GetRecv() {return bRecv; }
	void SetRecv(bool& bool_recv) { bRecv = bool_recv; }

private:
	std::unordered_map<int, shared_ptr<NetworkMoveObj>>m_obj_map;
	GameInfo m_game_info;

	bool bRecv;
	Vec3 recv_pos;
};