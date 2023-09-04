#include "pch.h"
#include "enemy.h"
using namespace std;



void Enemy::InitEnemy(OBJ_TYPE type, int room_id, Vector3& pos )
{
	m_type = type;
	m_room_id = room_id;
	SetPos(pos);
}

void Enemy::SetSpawnPoint(float x, float z)
{
	Vector3 pos(x, 0.0f, z);
	SetOriginPos(pos);
	SetPos(pos);
	m_prev_pos = pos;
}

void Enemy::Reset()
{
	target_id = BASE_ID;
	m_room_id = -1;
	m_attack_time = std::chrono::system_clock::now();
	m_check_time = std::chrono::system_clock::now();
	m_move_time = std::chrono::system_clock::now();
	in_use = false;
	in_game = false;
	m_is_active = false;
	m_prev_test_pos = Vector3{ 0.0f,0.0f,0.0f };
}

void Enemy::SetMoveTime(int val)
{
	m_attack_time = std::chrono::system_clock::now() + (1ms * val);
}

void Enemy::DoMove(const Vector3& target_pos)
{
	//// 현재 위치와 시선을 변수로 저장합니다.
	//Vector3& nlook = m_look;
	//Vector3& curr_pos = m_pos;

	//// 이전 위치를 저장합니다.
	//m_prev_pos = curr_pos; // "m_prev_pos = m_pos"를 "m_prev_pos = curr_pos"로 수정합니다.

	//// 시선을 새로운 대상 위치로 설정합니다.
	//nlook = target_pos;

	//// 대상 위치로부터의 이동 벡터를 계산합니다.
	//Vector3 move_vec = nlook - curr_pos; // "Vector3{ target_pos - curr_pos };"를 "Vector3 move_vec = nlook - curr_pos;"로 수정합니다.

	//// 이동 벡터를 정규화합니다.
	//move_vec.Normalize(); // "move_vec.Normalrize();"를 "move_vec.Normalize();"로 수정합니다.

	//// 현재 위치를 이동 벡터만큼 업데이트합니다.
	//Vector3 new_pos = curr_pos + move_vec;

	//m_pos = new_pos;
}

void Enemy::DoPrevMove(const Vector3& target_pos)
{
	Vector3& nlook = m_look;
	Vector3& curr_pos = m_prev_pos;
	nlook = Vector3{ target_pos - curr_pos };
	Vector3 move_vec = nlook.Normalrize();
	//Vector3 npos = curr_pos + (move_vec * MAX_SPEED);
	m_prev_pos = m_pos;
	//m_pos = npos;

}
