#pragma once
#include "pch.h"

class RayCasting
{
	RayCasting();
	~RayCasting();

private:
	// 총알 발사 위치
	Vector3 bulletPos;

	// 쏜 순간 플레이어 방향벡터
	Vector3 beulletDir;

	//광선의 방향벡터
	float ray;

	//시작점과 끝점만 인자로 받아서 선을 표시한다.
	//충돌 전까지 방향 벡터그대로 쭉 직진
public:
		bool Shoot();

};