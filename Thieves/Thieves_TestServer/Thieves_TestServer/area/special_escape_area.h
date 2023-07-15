#pragma once
class SSpawn_area
{
public:
	SSpawn_area() = default;
	SSpawn_area(float centerSSpawn_area[3], float extentSSpawn_area[3], float axisSSpawn_area[3][3], float translationSSpawn_area[3])
	{
		center[0] = centerSSpawn_area[0];
		center[1] = centerSSpawn_area[1];
		center[2] = centerSSpawn_area[2];

		extent[0] = extentSSpawn_area[0];
		extent[1] = extentSSpawn_area[1];
		extent[2] = extentSSpawn_area[2];

		axis[0][0] = axisSSpawn_area[0][0];
		axis[0][1] = axisSSpawn_area[0][1];
		axis[0][2] = axisSSpawn_area[0][2];

		axis[1][0] = axisSSpawn_area[1][0];
		axis[1][1] = axisSSpawn_area[1][1];
		axis[1][2] = axisSSpawn_area[1][2];

		axis[2][0] = axisSSpawn_area[2][0];
		axis[2][1] = axisSSpawn_area[2][1];
		axis[2][2] = axisSSpawn_area[2][2];

		translation[0] = translationSSpawn_area[0];
		translation[1] = translationSSpawn_area[1];
		translation[2] = translationSSpawn_area[2];
	}
	~SSpawn_area() {};

	bool BoxBoxIntersection2(const SSpawn_area& box0);
	bool Intersection2(const SSpawn_area& box0, int& collisionDirection);





	float center[3];
	float axis[3][3];
	float extent[3];
	float translation[3];
};