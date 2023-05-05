#pragma once
#include <math.h>
const float EPSILON = 0.0001;

class Vector2 {
public:
	float x, z; //������ 
	Vector2() {} Vector2(float _x, float _z) { x = _x, z = _z; }
	//���� 
	float cross(const Vector2& other) const {
		return (x * other.z) - (z * other.x);
	}
	/* ������ �����ε��� ���� ���� ������ ������ �����մϴ�. */
	//������ �Ǽ��� 
	Vector2 operator * (double r) const { return Vector2(x * r, z * r); }
	float operator * (Vector2 r) const { return (x * r.x) + (z * r.z); }
	//������ ���� 
	Vector2 operator + (Vector2 other) const {
		return Vector2(x + other.x, z + other.z);
	}
	//������ ���� 
	Vector2 operator - (Vector2 other) const {
		return Vector2(x - other.x, z - other.z);
	}
	float Dist2d(const Vector2& other)
	{
		float temp_x = (x - other.x) * (x - other.x);
		float temp_z = (z - other.z) * (z - other.z);
		return sqrt(temp_x + temp_z);
	}
	//�� ������ �� 
	bool operator == (Vector2 other) const {
		return x == other.x && z == other.z;
	} bool operator < (Vector2 other) const {
		return x < other.x && z < other.z;
	}
};

