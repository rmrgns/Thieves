#pragma once
#include "pch.h"

class Pos
{
public:
	Pos() = default;
	Pos(float centerPos[3])
	{
		center[0] = centerPos[0];
		center[1] = centerPos[1];
		center[2] = centerPos[2];
	}
	~Pos() {};

	float center[3];	

	float VecDotProduct2(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
};

inline float DotProduct(const float v0[3], const float v1[3])
{//=v0.v1
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

