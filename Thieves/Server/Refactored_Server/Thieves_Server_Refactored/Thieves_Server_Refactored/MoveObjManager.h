#pragma once
#include <array>
#include <memory>
#include <mutex>
#include "Player.h"
#include "Enemy.h"
#include "MoveObj.h"
#include "protocol.hpp"

class MoveObjManager
{
public:
	static MoveObjManager& GetInst()
	{
		static MoveObjManager instance;
		return instance;
	}

	MoveObjManager() = default;
	~MoveObjManager() = default;

	MoveObjManager(const MoveObjManager&) = delete;
	MoveObjManager& operator=(const MoveObjManager&) = delete;

	[[nodiscard]] std::shared_ptr<Player> GetPlayer(int id);
	[[nodiscard]] std::shared_ptr<Enemy> GetEnemy(int id);
	[[nodiscard]] std::shared_ptr<MoveObj> GetMoveObj(int id);

	[[nodiscard]] bool IsPlayer(int id) const { return (id >= 0) && (id < MAX_USER); }
	[[nodiscard]] bool IsNPC(int id) const { return (NPC_ID_START <= id) && (id < NPC_ID_END); }

	void InitPlayer();
	void InitNPC();

	void ActivePlayer(int sessionId);

	void RemovePlayer(int sessionId);

	void ClearObjects();

private:
	std::array<std::shared_ptr<MoveObj>, MAX_USER + MAX_NPC> m_moveobj_arr{};
};