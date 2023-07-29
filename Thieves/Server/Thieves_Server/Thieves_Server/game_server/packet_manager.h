#pragma once
#include "define.h"
#include <concurrent_queue.h>
#include <thread>
#include "CBox.h"
#include "ray/ray_casting.h"
#include "Timer.h"
#include "NavMeshLoader.h"

class MoveObjManager;
class DB;
class RoomManager;
class MapManager;
class Lobby;
class RayCasting;
class NavMeshLoader;

class PacketManager {
public:
	PacketManager();
	~PacketManager() = default;

	void Init();
	void ProcessPacket(int c_id, unsigned char* p);
	void ProcessAccept(HANDLE, SOCKET&, EXP_OVER*);
	void ProcessRecv(int, EXP_OVER*, DWORD);	
	void ProcessStunEnd(int c_id);
	void ProcessInvincibleEnd(int c_id);
	void ProcessOpenSafe(int r_id);
	void ProcessOpenEscape(int r_id);
	void ProcessOpenSpecialEscape(int r_id);
	void ProcessTimerStart(int r_id);

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
	void SendStun(int c_id, int obj_id); //obj_id가 c_id를 공격
	void SendStunEnd(int c_id);
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

	void SendBullet(int c_id, Vector3);
	void SendLoginFailPacket(int c_id, int reason);

	void SendTimerStart(int c_id, std::chrono::system_clock::time_point tp);

	void SendOpenSafe(int c_id);
	void SendOpenEscapeArea(int c_id, int r_id);
	void SendOpenSpecialEscapeArea(int c_id, int r_id);
	void SendInvincible(int c_id, int p_id);
	void SendInvincibleEnd(int c_id, int p_id); // c_id에게 p_id의 무적이 끝났음을 알림

	void SendGameTimerStart(int c_id);

	void End();
	void Disconnect(int c_id);
	bool IsRoomInGame(int room_id);
	void EndGame(int room_id);

	void SendItemInfo(int c_id, int item_id);

	void CreateDBThread();
	void DBThread();
	void ProcessDBTask(db_task& dt);
	void JoinDBThread();

	

	//------timer-------
	static timer_event SetTimerEvent(int obj_id, int target_id, EVENT_TYPE ev, int seconds);
	static timer_event SetTimerEvent(int obj_id, int target_id, int room_id, EVENT_TYPE ev, int seconds);
	void ProcessTimer(HANDLE hiocp);

	//------NPC--------
	void UpdateObjMove();
	void SpawnNPCTime(int en_id, int room_id);
	void SpawnNPC(int room_id);
	void DoNpcMove(int enemy_id, int room_id);
	bool CheckMoveOK(int enemy_id, int room_id);
	void CallStateMachine(int enemy_id, int room_id, const Vector3& base_pos);
	void CountTime(int room_id);
	void DoEnemyAttack(int enemy_id, int target_id, int room_id);
	void SendNPCAttackPacket(int c_id, int obj_id, int target_id);
	
	void ProcessEvent(HANDLE hiocp, timer_event& ev);
	
private:
	Lobby* m_Lobby;
	RoomManager* m_room_manager;
	DB* m_db;
	DB* m_db2;
	MapManager* m_map_manager;
	RayCasting* m_ray_casting;
	concurrency::concurrent_queue<db_task>m_db_queue;
	std::thread db_thread;
	Timer* m_Timer;
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
	void ProcessBullet(int c_id, unsigned char* p);



	void StartGame(int room_id);




};