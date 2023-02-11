#pragma once
#include "define.h"
#include <thread>

class PacketManager {
public:
	PacketManager();
	~PacketManager();

	void Init();
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessAccept(HANDLE, SOCKET&, EXP_OVER*);
	void ProcessRecv(int, EXP_OVER*, DWORD);

	// SendPacket
	void SendMovePacket(int c_id, int mover);
	void SendSignInOK(int c_id);
	void SendSignUpOK(int c_id);
	void SendPutObjPakcet(int c_id, int obj_id, OBJ_TYPE obj_type);
	void SendTime(int c_id, float round_time);
	void SendAttackPacket(int c_id, int room_id);
	void SendGameWin(int c_id);
	void SendGameDefeat(int c_id);
	void SendStun(int c_id, int obj_id);
	void SendPhasePacket(int c_id, int curr_phase);

	void End();
	void Disconnect(int c_id);
	bool isRoomInGame(int room_id);
	void EndGame(int room_id);
};