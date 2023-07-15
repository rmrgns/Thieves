#pragma once
class Item_area
{
public:
	Item_area() = default;
	Item_area(float centerItem_area[3], float extentItem_area[3], float axisItem_area[3][3], float translationItem_area[3])
	{
		center[0] = centerItem_area[0];
		center[1] = centerItem_area[1];
		center[2] = centerItem_area[2];

		extent[0] = extentItem_area[0];
		extent[1] = extentItem_area[1];
		extent[2] = extentItem_area[2];

		axis[0][0] = axisItem_area[0][0];
		axis[0][1] = axisItem_area[0][1];
		axis[0][2] = axisItem_area[0][2];

		axis[1][0] = axisItem_area[1][0];
		axis[1][1] = axisItem_area[1][1];
		axis[1][2] = axisItem_area[1][2];

		axis[2][0] = axisItem_area[2][0];
		axis[2][1] = axisItem_area[2][1];
		axis[2][2] = axisItem_area[2][2];

		translation[0] = translationItem_area[0];
		translation[1] = translationItem_area[1];
		translation[2] = translationItem_area[2];
	}
	~Item_area() {};

	bool BoxBoxIntersection2(const Item_area& box0);
	bool Intersection2(const Item_area& box0, int& collisionDirection);





	float center[3];
	float axis[3][3];
	float extent[3];
	float translation[3];
};