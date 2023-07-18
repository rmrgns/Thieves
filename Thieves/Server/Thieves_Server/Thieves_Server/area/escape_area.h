#pragma once
class Escape_area
{
public:
	Escape_area() = default;
	Escape_area(float centerEscape_area[3], float extentEscape_area[3], float axisEscape_area[3][3], float translationEscape_area[3])
	{
		center[0] = centerEscape_area[0];
		center[1] = centerEscape_area[1];
		center[2] = centerEscape_area[2];

		extent[0] = extentEscape_area[0];
		extent[1] = extentEscape_area[1];
		extent[2] = extentEscape_area[2];

		axis[0][0] = axisEscape_area[0][0];
		axis[0][1] = axisEscape_area[0][1];
		axis[0][2] = axisEscape_area[0][2];

		axis[1][0] = axisEscape_area[1][0];
		axis[1][1] = axisEscape_area[1][1];
		axis[1][2] = axisEscape_area[1][2];

		axis[2][0] = axisEscape_area[2][0];
		axis[2][1] = axisEscape_area[2][1];
		axis[2][2] = axisEscape_area[2][2];

		translation[0] = translationEscape_area[0];
		translation[1] = translationEscape_area[1];
		translation[2] = translationEscape_area[2];
	}
	~Escape_area() {};

	bool BoxBoxIntersection2(const Escape_area& box0);
	bool Intersection2(const Escape_area& box0, int& collisionDirection);





	float center[3];
	float axis[3][3];
	float extent[3];
	float translation[3];
};