#pragma once
#include "define.h"
#include<concurrent_queue.h>
#include <thread>
#include "CBox.h"

class MoveObjManager;
class DB;
class RoomManager;
class MapManager;
class Lobby;

class PacketManager {
public:
	PacketManager();
	~PacketManager() = default;

	void Init();
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessAccept(HANDLE, SOCKET&, EXP_OVER*);
	void ProcessRecv(int, EXP_OVER*, DWORD);	
	void SendMovePacket(int c_id, int mover);
	
	void SendLoginFailPacket(SOCKET&, int reason);
	void SendSignInOK(int c_id);
	void SendSignUpOK(int c_id);

	void SendPutObjPakcet(int c_id, int obj_id, OBJ_TYPE obj_type);
	void SendObjInfo(int c_id, int obj_id);
	void SendObjInfoEnd(int c_id);
	void SendTime(int c_id, float round_time);
	void SendAttackPacket(int c_id, int room_id);
	void SendGameWin(int c_id);
	void SendGameDefeat(int c_id);
	void SendStun(int c_id, int obj_id);
	void SendPhasePacket(int c_id);

	void SendLoadProgress(int c_id, int p_id, int progressed);

	void SendLoadEnd(int c_id, int p_id);

	void SendAllPlayerLoadEnd(int c_id);

	void SendEnterRoom(int c_id, int p_id);
	void SendEnterRoomOk(int c_id, int room_id);
	void SendLeaveRoom(int c_id, int p_id);
	void SendPlayerReady(int c_id, int p_id);
	void SendPlayerCancleReady(int c_id, int p_id);

	void SendRoomsDataForLobby(int c_id, int room_id);
	void SendRoomsDataForLobbyEnd(int c_id);
	void SendRoomsDataForRoom(int c_id, int p_id);
	void SendRoomsDataForRoomEnd(int c_id);
	void SendError(int c_id, int err_type, int err_val);
	void SendGameStart(int c_id);

	void End();
	void Disconnect(int c_id);
	bool IsRoomInGame(int room_id);
	void EndGame(int room_id);

	void CreateDBThread();
	void DBThread();
	void ProcessDBTask(db_task& dt);
	void JoinDBThread();

	void ProcessTimer(HANDLE hiocp);

private:
	Lobby* m_Lobby;
	RoomManager* m_room_manager;
	DB* m_db;
	MapManager* m_map_manager;
	concurrency::concurrent_queue<db_task>m_db_queue;
	std::thread db_thread;
	
	float	_speed = 700.f;


	void ProcessSignIn(int c_id, unsigned char* p);
	void ProcessSignUp(int c_id, unsigned char* p);
	void ProcessAttack(int c_id, unsigned char* p);
	void Hit(int c_id, int p_id);
	void ProcessMove(int c_id, unsigned char* p);
	void ProcessMatching(int c_id, unsigned char* p);
	void ProcessHit(int c_id, unsigned char* p);
	void ProcessGameStart(int c_id, unsigned char* p);
	void ProcessChangePhase(int c_id, unsigned char* p);
	void ProcessGetItem(int c_id, unsigned char* p);
	void ProcessUseItem(int c_id, unsigned char* p);
	void ChangePhase(int c_id);

	void ProcessLoadProgressing(int c_id, unsigned char* p);
	void ProcessLoadEnd(int c_id, unsigned char* p);
	void ProcessEnterRoom(int c_id, unsigned char* p);
	void ProcessLeaveRoom(int c_id, unsigned char* p);
	void ProcessPlayerReady(int c_id, unsigned char* p);
	void ProcessPlayerCancleReady(int c_id, unsigned char* p);
	void ProcessLogOut(int c_id, unsigned char* p);
	void ProcessRoomsDataInLobby(int c_id, unsigned char* p);
	void ProcessRoomsDataInRoom(int c_id, unsigned char* p);
	void ProcessDamageCheat(int c_id, unsigned char* p);
	void StartGame(int room_id);
};