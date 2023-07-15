#pragma once
#include "pch.h"

class CBox
{
public:
	CBox() = default;
	CBox(float centerCBox[3], float extentCBox[3], float axisCBox[3][3], float translationCBox[3])
	{
		center[0] = centerCBox[0];
		center[1] = centerCBox[1];
		center[2] = centerCBox[2];

		extent[0] = extentCBox[0]; 
		extent[1] = extentCBox[1];
		extent[2] = extentCBox[2];
		
		axis[0][0] = axisCBox[0][0];
		axis[0][1] = axisCBox[0][1];
		axis[0][2] = axisCBox[0][2];

		axis[1][0] = axisCBox[1][0];
		axis[1][1] = axisCBox[1][1];
		axis[1][2] = axisCBox[1][2];

		axis[2][0] = axisCBox[2][0];
		axis[2][1] = axisCBox[2][1];
		axis[2][2] = axisCBox[2][2];
		
		translation[0] = translationCBox[0];
		translation[1] = translationCBox[1];
		translation[2] = translationCBox[2];
	}
	~CBox() {};

	bool BoxBoxIntersection2(const CBox& box0);
	bool Intersection2(const CBox& box0, int& collisionDirection );




	
	float center[3];
	float axis[3][3];		
	float extent[3];		
	float translation[3];	
};

inline float DotProduct(const float v0[3], const float v1[3])
{//=v0.v1
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

