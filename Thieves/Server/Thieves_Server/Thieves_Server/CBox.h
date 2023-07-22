#pragma once
#include "pch.h"
#include <D3d12.h>
#include <DirectXmath.h>
#include <directXmath.h>
#include <d3d12.h>

//#include "DirectXTex.h"
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
	CBox(float centerCBox[3])
	{
		center[0] = centerCBox[0];
		center[1] = centerCBox[1];
		center[2] = centerCBox[2];
	}
	CBox(float centerCBox1[3], float centerCBox2[3],float centerCBox3[3],float centerCBox4[3])
	{
		centerDir[0][0] = centerCBox1[0];
		centerDir[0][1] = centerCBox1[1];
		centerDir[0][2] = centerCBox1[2];
			  	 
		centerDir[1][0] = centerCBox2[0];
		centerDir[1][1] = centerCBox2[1];
		centerDir[1][2] = centerCBox2[2];
			  	 
		centerDir[2][0] = centerCBox3[0];
		centerDir[2][1] = centerCBox3[1];
		centerDir[2][2] = centerCBox3[2];
			  	 
		centerDir[3][0] = centerCBox4[0];
		centerDir[3][1] = centerCBox4[1];
		centerDir[3][2] = centerCBox4[2];
	}			
	~CBox() {};

	bool BoxBoxIntersection2(const CBox& box0);
	bool Intersection2(const CBox& box0, int& collisionDirection );
	bool IntersectionRay(const CBox& box0);
	
	float center[3];
	float centerDir[4][3];
	float axis[3][3];		
	float extent[3];		
	float translation[3];	

	float VecDotProduct2(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
};

inline float DotProduct(const float v0[3], const float v1[3])
{//=v0.v1
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

