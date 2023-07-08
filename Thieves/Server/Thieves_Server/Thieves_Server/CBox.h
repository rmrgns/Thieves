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
	~CBox() {};

	bool BoxBoxIntersection(const CBox& box0, int& collisionDirection);
	bool BoxBoxIntersection2(const CBox& box0);
	bool BoxBoxIntersectionNormal(const CBox& box0, Vector3& collisionNormal, int& collisionDirection);
	bool Intersection(const CBox& box0, int& collisionDirection);
	bool Intersection2(const CBox& box0, int& collisionDirection, Vector3&);


	Vector3 CalculateSlidingVector(const CBox& playerBox, int collisionDirection, Vector3);
	Vector3 CalculateSlidingVector2(CBox& playerBox, Vector3 playerVelocity, Vector3 collisionNormal);
	Vector3 CalculateSlidingVector3(CBox& playerBox, Vector3 playerVelocity, Vector3 collisionNormal);
	Vector3 CalculateSliding(CBox& playerBox, Vector3 playerVelocity, int collisionDirection);
	


	
	float center[3];
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

//float DistanceToPlane(XMFLOAT3 point, XMFLOAT3 normal, XMFLOAT3 planePoint)
//{
//	XMVECTOR pointVec = XMLoadFloat3(&point);
//	XMVECTOR normalVec = XMLoadFloat3(&normal);
//	XMVECTOR planePointVec = XMLoadFloat3(&planePoint);
//	return XMVectorGetX(XMPlaneDotCoord(XMPlaneFormPointNormal(planePointVec, normalVec), pointVec));
//}
//float Dot(XMLOAT3 a, XMFLOAT3 b)
//{
//		{
//		XMVECTOR aVec = XMLoadFloat3(&a);
//		XMVECTOR bVec = XMLoadFloat3(&b);
//		return XMVectorGetX(XMVector3Dot(aVec, bVec));
//	}
//}
//
//if (other.Intersects(moved))
//{
//	UINT collidedFaceIndex = 0;
//	float minPenetrationDepth = FLT_MAX;
//	for (UINT i = 0; i < 6; i++)
//	{
//		XMFLOAT3 FaceNormal;
//		XMStoreFloat3(&faceNormal, OrientedWorldNormals[i]);
//		float distanceToPlane DistanceToPlane(other.Center, faceNormal, moved.Center);
//		if (distanceToPlane > 0.0f)
//		{
//			float penetration = ((other.Extents.x *  abs(faceNormal.x)) +
//				(other.Extents.y * abs(faceNormal.y)) +
//				(other.Extents.z * abs(faceNormal.z))) - distanceToPlane;
//			if (penetration < minPenetrationDepth)
//			{
//				collidedFaceIndex = i;
//				minPenetrationDepth = penetration;
//				collisioninfo.collision_face_num = i;
//
//			}
//		}
//	}
//
//	XMFLOAT3 temp;
//	temp.x = moved.Center.x - other.Center.x;
//	temp.y = moved.Center.y - other.Center.y;
//	temp.z = moved.Center.z - other.Center.z;
//
//	XMStoreFloat3(&collisionNormal, OrientedWorldNormals[collidedFaceIndex]);
//	if (Dot(collision Normal, temp) < 0.0f)
//	{
//		collisionNormal.x *= -1;
//		collisionNormal.y *= -1;
//		collisionNormal.z *= -1;
//	}
//}
//#define fc_fabs fabsf

