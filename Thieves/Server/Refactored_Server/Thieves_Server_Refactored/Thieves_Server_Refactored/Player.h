#pragma once
#include "MoveObj.h"
#include "State.h"
#include <atomic>
#include <mutex>

class Player : public MoveObj
{
public:
	Player();
	virtual ~Player() = default;
	std::mutex state_lock;

	std::atomic<bool> is_matching{ false };

	[[nodiscard]] P_STATE GetState() const { return m_state; }
	void SetState(P_STATE val) { m_state = val; }

	// 네트워크 기능(DoRecv, DoSend, Init(SOCKET), m_socket 등)은 
	// Session 클래스가 담당하므로 전부 지우기.

	void ResetPlayer();
	virtual void Reset() override;

	void SetIsReady(bool val) { m_is_ready.store(val, std::memory_order_release); }
	[[nodiscard]] bool GetIsReady() const { return m_is_ready.load(std::memory_order_acquire); }

	void SetLoadProgressed(int val) { m_loadProgressed = val; }
	[[nodiscard]] int GetLoadProgressed() const { return m_loadProgressed; }

	void SetAttacked(bool val) { m_is_attacked.store(val, std::memory_order_release); }
	[[nodiscard]] bool GetAttacked() const { return m_is_attacked.load(std::memory_order_acquire); }

	void SetInvincible(bool val) { m_invincible.store(val, std::memory_order_release); }
	[[nodiscard]] bool GetInvincible() const { return m_invincible.load(std::memory_order_acquire); }

	[[nodiscard]] char* GetPassword() { return m_password; }

	[[nodiscard]] short GetMatchUserSize() const { return m_mach_user_size; }
	void SetMatchUserSize(short val) { m_mach_user_size = val; }

	void SetItem(int val) { m_Item.store(val, std::memory_order_release); }
	[[nodiscard]] int GetItem() const { return m_Item.load(std::memory_order_acquire); }
	void DropItem() { m_Item.store(-1, std::memory_order_release); }

	void SetHasDiamond(bool val) { m_has_diamond.store(val, std::memory_order_release); }
	[[nodiscard]] bool GetHasDiamond() const { return m_has_diamond.load(std::memory_order_acquire); }

private:
	P_STATE m_state{ P_STATE::ST_NONE };

	char m_password[MAX_PASSWORD_SIZE + 1]{};
	short m_mach_user_size{ 0 };

	// 인게임 로딩 중 활용할 데이터
	int m_loadProgressed{ 0 };

	// 멀티스레드 환경에서 안전하게 접근하기 위한 아토믹 변수
	std::atomic<bool> m_is_ready{ false };
	std::atomic<bool> m_is_attacked{ false };
	std::atomic<int> m_Item{ -1 };
	std::atomic<bool> m_has_diamond{ false };
	std::atomic<bool> m_invincible{ false };
	std::atomic<bool> m_isAbleToOpenSafe{ false };
};