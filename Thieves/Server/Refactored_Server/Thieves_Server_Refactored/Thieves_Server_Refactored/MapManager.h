#pragma once
#include <vector>
#include <memory>
#include "CBox.h"
#include "vec3.h"

class MapManager
{
public:
	static MapManager& GetInst()
	{
		static MapManager instance;
		return instance;
	}

	MapManager(const MapManager&) = delete;
	MapManager& operator=(const MapManager&) = delete;

	void LoadMap();
	void LoadSpawnArea();
	void LoadPoliceDir();
	void LoadItemSpawnPoint();
	void LoadEscapePoint();
	void LoadSpecialEscapePoint();
	void LoadPlayerSpawnArea();

	// 배열 원본을 그대로 반환하면 외부에서 실수로 수정할 위험이 있으므로,
	// 읽기 전용(const 참조자)으로 넘겨주도록 안전하게 바꾸기.
	[[nodiscard]] const std::vector<Vector3>& GetItemPos() const { return ItemPos; }
	[[nodiscard]] const std::vector<Vector3>& GetEscapePos() const { return EscapePos; }
	[[nodiscard]] const std::vector<Vector3>& GetSpecialEscapePos() const { return SpecialEscapePos; }
	[[nodiscard]] const std::vector<Vector3>& GetPlayerSpawnPos() const { return PlayerSpawnPos; }
	[[nodiscard]] const std::vector<Vector3>& GetNPCSpawnPos() const { return PoliceSpawnCBox; }

	[[nodiscard]] Vector3 checkCollision(CBox& playerBox, Vector3& playerOldPos);

	// 내부 상태를 변경하지 않는 함수에 const
	[[nodiscard]] bool checkCollisionRay(CBox& ray_temp) const;
	[[nodiscard]] bool checkCollisionSafe(CBox& ray_temp) const;

	[[nodiscard]] bool CheckInRange2(float x, float z, const Vector3& min_pos, const Vector3& max_pos) const
	{
		return (min_pos.x <= x && x <= max_pos.x && min_pos.z <= z && z <= max_pos.z);
	}

private:
	MapManager() = default;
	~MapManager() = default;

	std::vector<std::shared_ptr<CBox>> MapCBox;
	std::vector<Vector3> PoliceSpawnCBox;
	std::vector<std::shared_ptr<CBox>> PoliceDirCBox;
	std::vector<Vector3> ItemPos;
	std::vector<Vector3> EscapePos;
	std::vector<Vector3> SpecialEscapePos;
	std::vector<Vector3> PlayerSpawnPos;

	std::vector<Vector3> PlayerPos;

	CBox m_box_manager;
};