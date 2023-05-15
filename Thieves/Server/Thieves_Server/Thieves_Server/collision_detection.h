#pragma once
#include <math.h>

#include "CollisionDetection.hpp"

class CollisionDetection
{
public:
	CollisionDetection() {};
	~CollisionDetection() {};

	int BoxBoxIntersectionTest(const CBox& box0, const CBox& box1);

};