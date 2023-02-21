#include "packet_manager.h"

PacketManager::PacketManager()
{
}

void PacketManager::Init()
{
}

void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessAccept(HANDLE, SOCKET&, EXP_OVER*)
{
}

void PacketManager::ProcessRecv(int, EXP_OVER*, DWORD)
{
}

void PacketManager::SendMovePacket(int c_id, int mover)
{
}

void PacketManager::SendSignInOK(int c_id)
{
}

void PacketManager::SendSignUpOK(int c_id)
{
}

void PacketManager::SendPutObjPakcet(int c_id, int obj_id, OBJ_TYPE obj_type)
{
}

void PacketManager::SendTime(int c_id, float round_time)
{
}

void PacketManager::SendAttackPacket(int c_id, int room_id)
{
}

void PacketManager::SendGameWin(int c_id)
{
}

void PacketManager::SendGameDefeat(int c_id)
{
}

void PacketManager::SendStun(int c_id, int obj_id)
{
}

void PacketManager::SendPhasePacket(int c_id, int curr_phase)
{
}

void PacketManager::End()
{
}

void PacketManager::Disconnect(int c_id)
{
}

bool PacketManager::IsRoomInGame(int room_id)
{
	return false;
}

void PacketManager::EndGame(int room_id)
{
}

void PacketManager::CreateDBThread()
{
}

// 비동기로 DB작업을 실행하는 Thread
void PacketManager::DBThread()
{
}

// case에 따른 DB 로그인 가입을 판단하는 함수
void PacketManager::ProcessDBTask(db_task& dt)
{
}

void PacketManager::JoinDBThread()
{
}

void PacketManager::ProcessTimer(HANDLE hiocp)
{
}

void PacketManager::ProcessSignIn(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessSignUp(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessAttack(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessMove(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessMatching(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessHit(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessGameStart(int c_id, unsigned char* p)
{
}

void PacketManager::ProcessDamageCheat(int c_id, unsigned char* p)
{
}

void PacketManager::StartGame(int room_id)
{
}
