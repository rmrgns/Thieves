#pragma once
class Spawn_area
{
public:
	Spawn_area() = default;
	Spawn_area(float centerSpawn_area[3], float extentSpawn_area[3], float axisSpawn_area[3][3], float translationSpawn_area[3])
	{
		center[0] = centerSpawn_area[0];
		center[1] = centerSpawn_area[1];
		center[2] = centerSpawn_area[2];

		extent[0] = extentSpawn_area[0];
		extent[1] = extentSpawn_area[1];
		extent[2] = extentSpawn_area[2];

		axis[0][0] = axisSpawn_area[0][0];
		axis[0][1] = axisSpawn_area[0][1];
		axis[0][2] = axisSpawn_area[0][2];

		axis[1][0] = axisSpawn_area[1][0];
		axis[1][1] = axisSpawn_area[1][1];
		axis[1][2] = axisSpawn_area[1][2];

		axis[2][0] = axisSpawn_area[2][0];
		axis[2][1] = axisSpawn_area[2][1];
		axis[2][2] = axisSpawn_area[2][2];

		translation[0] = translationSpawn_area[0];
		translation[1] = translationSpawn_area[1];
		translation[2] = translationSpawn_area[2];
	}
	~Spawn_area() {};

	bool BoxBoxIntersection2(const Spawn_area& box0);
	bool Intersection2(const Spawn_area& box0, int& collisionDirection);





	float center[3];
	float axis[3][3];
	float extent[3];
	float translation[3];
};