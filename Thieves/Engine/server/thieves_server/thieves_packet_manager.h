#pragma once
#include <unordered_map>

#include "server/main/packet_manager.h"
#include "server/main/network_obj_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"
#include "PlayerInput.h"
#include "MapItem.h"


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
	void ProcessBullet(int c_id, unsigned char* p);
	void ProcessPhaseChange(int c_id, unsigned char* p);
	void ProcessAttack(int c_id, unsigned char* p);
	void ProcessHit(int c_id, unsigned char* p);
	void ProcessStunEnd(int c_id, unsigned char* p);
	void ProcessGetItem(int c_id, unsigned char* p);
	void ProcessItemUse(int c_id, unsigned char* p);
	void ProcessItemInfo(int c_id, unsigned char* p);
	void ProcessTimerStart(int c_id, unsigned char* p);
	void ProcessActiveEscape(int c_id, unsigned char* p);
	void ProcessActiveSpecialEscape(int c_id, unsigned char* p);
	void ProcessOpenSafe(int c_id, unsigned char* p);
	void ProcessInvincible(int c_id, unsigned char* p);
	void ProcessInvincibleEnd(int c_id, unsigned char* p);
	void ProcessGameTimerStart(int c_id, unsigned char* p);
	void ProcessInteract(int c_id, unsigned char* p);
	void ProcessDiamondOwnerChange(int c_id, unsigned char* p);
	void ProcessWin(int c_id, unsigned char* p);
	void ProcessDefeat(int C_id, unsigned char* p);

	void ProcessTime(int c_id, unsigned char* p);
	void ProcessNpcAttack(int c_id, unsigned char* p);
	// �߰��ʿ�
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
	std::unordered_map<int, shared_ptr<MapItem>>& GetItemObjMap() { return m_item_map; }
	GameInfo& GetGameInfo() { return m_game_info; };

	void SetDiamondPlayer(int val) { m_diamond_player = val; }
	int GetDiamondPlayer() { return m_diamond_player; }

private:
	std::unordered_map<int, shared_ptr<NetworkMoveObj>>m_obj_map;
	std::unordered_map<int, shared_ptr<MapItem>>m_item_map;
	GameInfo m_game_info;
	int m_diamond_player = -1;

	char pl_action;
	bool bRecv;
	Vec3 recv_pos;

	bool particleCheck = false;
	float particleTime = 0.f;
	//Vec3 recv_pos;
};