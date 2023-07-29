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
	Vector3& nlook = m_look;
	Vector3& curr_pos = m_pos;
	m_prev_pos = m_pos;
	nlook = target_pos;//Vector3{ target_pos - curr_pos };
//	Vector3 move_vec = nlook.Normalrize();
//	Vector3 npos = curr_pos + (move_vec);
//	m_pos = npos;

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
