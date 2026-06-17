#include "Player.h"
#include <algorithm>

Player::Player()
{
	m_type = OBJ_TYPE::OT_PLAYER;
	m_state = P_STATE::ST_NONE;
	m_room_id = 0;
	m_last_move_time = 0;
}

void Player::ResetPlayer()
{
	SetPos(Vector3());
	SetRot(Vector3());

	m_is_active.store(false, std::memory_order_release);
	is_matching.store(false, std::memory_order_release);

	// ZeroMemory 대신 C++ 표준인 std::fill을 사용해서 배열 비우기
	std::fill(std::begin(m_password), std::end(m_password), '\0');

	m_mach_user_size = 0;
	m_Item.store(-1, std::memory_order_release);
	m_loadProgressed = 0;

	m_is_attacked.store(false, std::memory_order_release);
	m_has_diamond.store(false, std::memory_order_release);
	m_invincible.store(false, std::memory_order_release);
	m_isAbleToOpenSafe.store(false, std::memory_order_release);
	m_is_ready.store(false, std::memory_order_release);
}

void Player::Reset()
{
	m_is_active.store(false, std::memory_order_release);
	m_mach_user_size = 0;
	m_last_move_time = 0;
	m_room_id = 1;

	m_is_ready.store(false, std::memory_order_release);
	is_matching.store(false, std::memory_order_release);

	// std::lock_guard를 쓰면 블록이 끝날 때 알아서 unlock을 해줌
	std::lock_guard<std::mutex> lock(state_lock);
	m_state = P_STATE::ST_LOGIN;
}
