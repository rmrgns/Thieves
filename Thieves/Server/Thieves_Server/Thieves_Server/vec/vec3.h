#pragma once
#include <iostream>
#include <ostream>
#include <cmath>

using Vec2 = std::pair<short, short>;

class Vector3
{
public:
	float x, y, z;

//#ifndef operation_overloading
	friend std::ostream& operator<< (std::ostream& os, const Vector3& a)

	{
		return os << "{" << a.x << ", " << a.y << ", " << a.z << "}";
	}

	friend Vector3 operator+(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	friend Vector3 operator-(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	friend void operator+=(Vector3& a, const Vector3& b)
	{
		a.x += b.x;
		a.y += b.y;
		a.z += b.z;
	}

	friend bool operator==(const Vector3& a, const Vector3& b)
	{
		return (a.x == b.x && a.y == b.y && a.z == b.z);
	}

	friend bool operator!=(const Vector3& a, const Vector3& b)
	{
		return (a.x != b.x || a.y != b.y || a.z != b.z);
	}

	friend Vector3 operator*(const Vector3& a, const float b)
	{
		return Vector3(a.x * b, a.y * b, a.z * b);
	}

	friend Vector3 operator*(const float b, const Vector3& a)
	{
		return Vector3(a.x * b, a.y * b, a.z * b);
	}

	friend Vector3 operator/(const Vector3& a, const float b)
	{
		return Vector3(a.x / b, a.y / b, a.z / b);
	}

//#endif

	float VectorScale() const
	{
		float temp_x = x * x;
		float temp_y = y * y;
		float temp_z = z * z;
		float scale = sqrt(temp_x + temp_y + temp_z);

		return scale;
	}

	Vector3 Normalize() const
	{
		float scale = VectorScale();
		return Vector3(x / scale, y / scale, z / scale);
	}

	float Dist(const Vector3& other) const
	{
		float temp_x = (x - other.x) * (x - other.x);
		float temp_y = (y - other.y) * (y - other.y);
		float temp_z = (z - other.z) * (z - other.z);
		return sqrt(temp_x + temp_y + temp_z);
	}

	float Dist2d(const Vector3& other) const
	{
		float temp_x = (x - other.x) * (x - other.x);
		float temp_z = (z - other.z) * (z - other.z);
		return sqrt(temp_x + temp_z);
	}

	float Dot(const Vector3& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3 Cross(const Vector3& other)
	{
		return Vector3(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	Vector3 ProjectOnVector(const Vector3& vector) const
	{
		float dotProduct = Dot(vector);
		float vectorLength = vector.VectorScale();
		Vector3 projection = (dotProduct / (vectorLength * vectorLength)) * vector;
		return projection;
	}
	Vector3 Normalrize()
	{
		float scale = VectorScale();
		Vector3 nor{ x / scale, y / scale, z / scale };
		return nor;
	}
};

