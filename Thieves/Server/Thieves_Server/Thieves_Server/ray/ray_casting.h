#pragma once
#include "pch.h"
#include "define.h"
#include "object/MapManager.h"

class RayCasting
{
	RayCasting();
	~RayCasting() {};

	float center[3];
	float axis[3][3];
	float extent[3];
	float translation[3];

private:
	// 총알 발사 위치
	Vector3 bulletPos;

	// 쏜 순간 플레이어 방향벡터
	Vector3 beulletDir;

	// 충돌 지점 저장할 변수
	Vector3 collisionPoint;

	//광선의 방향벡터
	float ray;

	//시작점과 끝점만 인자로 받아서 선을 표시한다.
	//충돌 전까지 방향 벡터그대로 쭉 직진

public:
	//MapManager* _map_manager;
	
	
	Vector3 Shoot(Vector3 RayPos, Vector3 RayDir);
	bool CheckSafe(Vector3 RayPos, Vector3 RayDir);
public:
	float GetBulletX()const { return bulletPos.x; };
	float GetBulletY()const { return bulletPos.y; };
	float GetBulletZ()const { return bulletPos.z; };
	Vector3 GetbulletPos()const { return bulletPos; };

	void SetBulletX(float val) { bulletPos.x = val; };
	void SetBulletY(float val) { bulletPos.y = val; };
	void SetBulletZ(float val) { bulletPos.z = val; };
	void SetbulletPos(Vector3 val) { bulletPos = val; };

	void SetbulletDir(Vector3 val) { beulletDir = val; };
	Vector3 GetbulletDir()const { return beulletDir; };
	void SetcollisionPoint(Vector3 val) { collisionPoint = val; };
	Vector3 GetcollisionPoint()const { return collisionPoint; };

};