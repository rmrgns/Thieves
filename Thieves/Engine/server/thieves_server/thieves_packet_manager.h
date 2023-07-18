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
	
	void ProcessMove(int c_id, unsigned char* p);
	void ProcessSignin(int c_id, unsigned char* p);
	void ProcessObjInfo(int c_id, unsigned char* p);
	void ProcessObjInfoEnd(int c_id, unsigned char* p);
	void ProcessRemoveObj(int c_id, unsigned char* p);
	void ProcessRoomsData(int c_id, unsigned char* p);
	void ProcessRoomsDataEnd(int c_id, unsigned char* p);
	void ProcessInRoomData(int c_id, unsigned char* p);
	void ProcessInRoomDataEnd(int c_id, unsigned char* p);
	void ProcessPlayerLeft(int c_id, unsigned char* p);
	void ProcessPlayerJoin(int c_id, unsigned char* p);
	void ProcessPlayerReady(int c_id, unsigned char* p);
	void ProcessPlayerCancleReady(int c_id, unsigned char* p);
	void ProcessEnterRoomOk(int c_id, unsigned char* p);
	void ProcessAllPlayerLoadend(int c_id, unsigned char* p);
	void ProcessError(int c_id, unsigned char* p);
	void ProcessGameStart(int c_id, unsigned char* p);

	// 추가필요
	void Reset();

	Vec3 GetVec(){ return recv_pos; }

	void SetVecX(float& p_posx) { recv_pos.x = p_posx; }
	void SetVecY(float& p_posy) { recv_pos.y = p_posy; }
	void SetVecZ(float& p_posz) { recv_pos.z = p_posz; }
	
	Vec3 GetPlayerInput() { return recv_pos; }
	bool GetRecv() {return bRecv; }
	void SetRecv(bool& bool_recv) { bRecv = bool_recv; }
	
	char GetActionType() { return pl_action; }
	void SetActionType(char type_val) { pl_action = type_val; }

	std::unordered_map<int, shared_ptr<NetworkMoveObj>>& GetObjMap() { return m_obj_map; };
	GameInfo& GetGameInfo() { return m_game_info; };

private:
	std::unordered_map<int, shared_ptr<NetworkMoveObj>>m_obj_map;
	GameInfo m_game_info;

	char pl_action;
	bool bRecv;
	Vec3 recv_pos;
	//Vec3 recv_pos;
};