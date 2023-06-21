#include "pch.h"
#include "CBox.h"

// 충돌검사
bool CBox::BoxBoxIntersection(const CBox& box0, int& collisionDirection)
{
	// compute difference of box centers,D=C1-C0
	float D[3] =
	{
	 center[0] - box0.center[0],
	 center[1] - box0.center[1],
	 center[2] - box0.center[2]
	};

	float C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
	float absC[3][3]; //|c_{ij}|
	float AD[3];      //Dot(A_i,D)
	float R0, R1, R;  //interval radii and distance between centers
	float R01;        //=R0+R1

	// box0.axis 축과의 충돌체크
	// 
	//A0
	// 1
	C[0][0] = DotProduct(box0.axis[0],axis[0]);
	C[0][1] = DotProduct(box0.axis[0],axis[1]);
	C[0][2] = DotProduct(box0.axis[0],axis[2]);

	AD[0] = DotProduct(box0.axis[0], D);
	absC[0][0] = (float)fabsf(C[0][0]);
	absC[0][1] = (float)fabsf(C[0][1]);
	absC[0][2] = (float)fabsf(C[0][2]);
	R = (float)fabsf(AD[0]);
	R1 = extent[0] * absC[0][0] + extent[1] * absC[0][1] + extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;
	if (R > R01) {
		//front
		collisionDirection = 1;
		return false;
	}
	
	//A1
	C[1][0] = DotProduct(box0.axis[1], axis[0]);
	C[1][1] = DotProduct(box0.axis[1], axis[1]);
	C[1][2] = DotProduct(box0.axis[1], axis[2]);
	AD[1] = DotProduct(box0.axis[1], D);
	absC[1][0] = (float)fabsf(C[1][0]);
	absC[1][1] = (float)fabsf(C[1][1]);
	absC[1][2] = (float)fabsf(C[1][2]);
	R = (float)fabsf(AD[1]);
	R1 = extent[0] * absC[1][0] + extent[1] * absC[1][1] + extent[2] * absC[1][2];
	R01 = box0.extent[1] + R1;
	if (R > R01) {
		collisionDirection = 2;
		return false;
	}

	//A2
	C[2][0] = DotProduct(box0.axis[2], axis[0]);
	C[2][1] = DotProduct(box0.axis[2], axis[1]);
	C[2][2] = DotProduct(box0.axis[2], axis[2]);
	AD[2] = DotProduct(box0.axis[2], D);
	absC[2][0] = (float)fabsf(C[2][0]);
	absC[2][1] = (float)fabsf(C[2][1]);
	absC[2][2] = (float)fabsf(C[2][2]);
	R = (float)fabsf(AD[2]);
	R1 = extent[0] * absC[2][0] + extent[1] * absC[2][1] + extent[2] * absC[2][2];
	R01 = box0.extent[2] + R1;
	if (R > R01) {
		collisionDirection = 3;
		return false;
	}

	// axis 축과의 충돌체크
	//B0
	R = (float)fabsf(DotProduct(axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01) {
		collisionDirection = 1;
		return false;
	}

	//B1
	R = (float)fabsf(DotProduct(axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01) {
		collisionDirection = 2;
		return false;
	}

	//B2
	R = (float)fabsf(DotProduct(axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01) {
		collisionDirection = 3;
		return false;
	}

	//axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	//A0xB0
	R = (float)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 1;
		return false;
	}

	//A0xB1
	R = (float)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 2;
		return false;
	}

	//A0xB2
	R = (float)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 3;
		return false;
	}

	//A1xB0
	R = (float)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 1;
		return false;
	}

	//A1xB1
	R = (float)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 2;
		return false;
	}
	//A1xB2
	R = (float)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 3;
		return false;
	}

	//A2xB0
	R = (float)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 1;
		return false;
	}

	//A2xB1
	R = (float)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 2;
		return false;
	}

	//A2xB2
	R = (float)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		collisionDirection = 3;
		return false;
	}

	// 충돌 안했어
	return true;
}

//Vector3 CBox::CalculateSlidingVector(const CBox& playerBox, int collisionDirection, Vector3 playerVelocity)
//{
//	
//	Vector3 slidingVector{0.0f, 0.0f, 0.0f};
//	// 충돌 방향 벡터 계산	
//	float collisionNormal[3] = { 0.0f, 0.0f, 0.0f };
//	collisionNormal[collisionDirection - 1] = 1.0f; // 충돌 방향을 나타내는 축을 1로 설정
//
//	// 슬라이딩 벡터 계산
//	float dotProduct = DotProduct(playerBox.center, collisionNormal) - DotProduct(center, collisionNormal);
//	
//	//DotProduct(playerVelocity, collisionNormal);
//	slidingVector.x = collisionNormal[0] * dotProduct/40;
//	slidingVector.y = 0;//collisionNormal[1] * dotProduct/70;
//	slidingVector.z = collisionNormal[2] * dotProduct/40;
//	//slidingVector.x = collisionNormal[0];
//	//slidingVector.y = collisionNormal[1];
//	//slidingVector.z = collisionNormal[2];
//
//	
//
//	return slidingVector;
//}

Vector3 CBox::CalculateSlidingVector(const CBox& playerBox, int collisionDirection, Vector3 playerVelocity)
{
	Vector3 slidingVector{ 0.0f, 0.0f, 0.0f };

	// 충돌 방향에 따라 충돌 법선 벡터를 계산합니다.
	Vector3 collisionNormal{ 0.0f, 0.0f, 0.0f };
	if (collisionDirection == 1) {
		collisionNormal.x = 1.0f;
	}
	else if (collisionDirection == 2) {
		collisionNormal.y = 1.0f;
	}
	else if (collisionDirection == 3) {
		collisionNormal.z = 1.0f;
	}

	// 플레이어 속도와 충돌 법선의 내적을 계산합니다.
	//float dotProduct = playerVelocity.Dot(collisionNormal);
	float dotProduct = playerVelocity.x * collisionNormal.x + playerVelocity.y * collisionNormal.y + playerVelocity.z * collisionNormal.z;

	// 슬라이딩 벡터를 계산합니다.
	slidingVector.x = playerVelocity.x - 2.0f * dotProduct * collisionNormal.x;
	slidingVector.y = playerVelocity.y - 2.0f * dotProduct * collisionNormal.y;
	slidingVector.z = playerVelocity.z - 2.0f * dotProduct * collisionNormal.z;

	return slidingVector;
}

Vector3 CBox::CalculateSliding(CBox& playerBox, Vector3 playerVelocity, int collisionDirection)
{
	Vector3 slidingVector;

	// 충돌지점계산
//	Vector3 collisionPoint;
//	collisionPoint.x = (previousPositionA.x + currentPositionA.x);
//	collisionPoint.y = (previousPositionA.y + currentPositionA.y);
//	collisionPoint.z = (previousPositionA.z + currentPositionA.z);

	// 충돌한 두 객체의 상대 속도 계산
	Vector3 relativeVelocity;
//	relativeVelocity.x = currentPositionB.x - currentPositionA.x;
//	relativeVelocity.y = currentPositionB.y - currentPositionA.y;
//	relativeVelocity.z = currentPositionB.z - currentPositionA.z;

	relativeVelocity = playerVelocity;

	// *2를 넣고 normallize 해야됨.


	// 충돌 법선 벡터 계산
	Vector3 collisionNormal;

	if ( collisionDirection == 1 ) {
			collisionNormal = { 1, 0, 0 };
	}
	else if ( collisionDirection == 2 ) {
			collisionNormal = {0, 1, 0};
	}
	else if ( collisionDirection == 3 ) {
			collisionNormal = {1, 0, 0};
	}

	collisionDirection = 0;
	collisionNormal.Normalrize();
	// 슬라이딩 벡터 계산
	float dotProduct = relativeVelocity.Dot(collisionNormal);
	Vector3 scaledNormal = collisionNormal * dotProduct;
	slidingVector = relativeVelocity - scaledNormal;

	return slidingVector;
}