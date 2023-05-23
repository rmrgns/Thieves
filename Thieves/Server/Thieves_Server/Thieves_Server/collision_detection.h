#pragma once
#include <math.h>
#include "CBox.h"

class CollisionDetection
{
public:
	CollisionDetection() {};
	~CollisionDetection() {};

	int BoxBoxIntersectionTest(const CBox& box0, const CBox& box1);

private:

};