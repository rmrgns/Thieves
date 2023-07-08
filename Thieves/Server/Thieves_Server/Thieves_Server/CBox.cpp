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
		
		return false;
	}

	// axis 축과의 충돌체크
	//B0
	R = (float)fabsf(DotProduct(axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01) {
	
		return false;
	}

	//B1
	R = (float)fabsf(DotProduct(axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01) {
		
		return false;
	}

	//B2
	R = (float)fabsf(DotProduct(axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01) {
		
		return false;
	}

	//axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	//A0xB0
	R = (float)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) {
	
		return false;
	}

	//A0xB1
	R = (float)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
	
		return false;
	}

	//A0xB2
	R = (float)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		
		return false;
	}

	//A1xB0
	R = (float)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) {
		
		return false;
	}

	//A1xB1
	R = (float)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
	
		return false;
	}
	//A1xB2
	R = (float)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		
		return false;
	}

	//A2xB0
	R = (float)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) {
		
		return false;
	}

	//A2xB1
	R = (float)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
	
		return false;
	}

	//A2xB2
	R = (float)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		
		return false;
	}
	return true;
}

bool CBox::BoxBoxIntersection2(const CBox& box0)
{
	// 상자 중심의 차이인 D를 계산합니다.
	float D[3] =
	{
	center[0] - box0.center[0],
	center[1] - box0.center[1],
	center[2] - box0.center[2]
	};

	float C[3][3];    // 행렬 C=A^T B,c_{ij}=Dot(A_i,B_j)
	float absC[3][3]; // |c_{ij}|
	float AD[3];      // Dot(A_i,D)
	float R0, R1, R;  // 구간의 반경과 중심 사이의 거리
	float R01;        //=R0+R1

	// box0.axis 축과의 충돌체크
	// 
	// A0
	// 1
	C[0][0] = DotProduct(box0.axis[0], axis[0]);
	C[0][1] = DotProduct(box0.axis[0], axis[1]);
	C[0][2] = DotProduct(box0.axis[0], axis[2]);

	AD[0] = DotProduct(box0.axis[0], D);
	absC[0][0] = (float)fabsf(C[0][0]);
	absC[0][1] = (float)fabsf(C[0][1]);
	absC[0][2] = (float)fabsf(C[0][2]);
	R = (float)fabsf(AD[0]);
	R1 = extent[0] * absC[0][0] + extent[1] * absC[0][1] + extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;
	if (R > R01) {

		return false;
	}

	// A1
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

		return false;
	}

	// A2
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

		return false;
	}

	// axis 축과의 충돌체크
	// B0
	R = (float)fabsf(DotProduct(axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01) {

		return false;
	}

	// B1
	R = (float)fabsf(DotProduct(axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01) {

		return false;
	}

	// B2
	R = (float)fabsf(DotProduct(axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01) {

		return false;
	}

	// axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	// A0xB0
	R = (float)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) {

		return false;
	}

	// A0xB1
	R = (float)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {

		return false;
	}

	// A0xB2
	R = (float)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A1xB0
	R = (float)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A1xB1
	R = (float)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A1xB2
	R = (float)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A2xB0
	R = (float)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A2xB1
	R = (float)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A2xB2
	R = (float)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// 모든 축과 외적 및 내적 검사를 통과한 경우, 두 상자는 교차합니다.
	return true;
}


bool CBox::BoxBoxIntersectionNormal(const CBox& playerBox, Vector3& collisionN, int& collisionDirection)
{
	bool collideRet = FALSE;
	// compute difference of box centers,D=C1-C0
	// 중심 사이의 거리
	float D[3] =
	{
		center[0] - playerBox.center[0],
		center[1] - playerBox.center[1],
		center[2] - playerBox.center[2]
	};
	// 두 상자의 축을 나타내는 행렬 C
	// player i 번째 축벡터와 box의 j번째 축 벡터의 내적
	float C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)

	// C의 절댓값
	float absC[3][3]; //|c_{ij}|

	// player의 축과 D의 내적  = AD
	float AD[3];      //Dot(A_i,D)
	// 각 축에 대해 축 벡터의 길이와 absC를 사용하여 R0 R1을 계산
	// R은 AD의 절댓값
	// R0와 R1의 합 = R01
	float R0, R1, R;  //interval radii and distance between centers
	float R01;        //=R0+R1

	// playerBox.axis 축과의 충돌체크
	//A0
	C[0][0] = DotProduct(playerBox.axis[0], axis[0]);
	C[0][1] = DotProduct(playerBox.axis[0], axis[1]);
	C[0][2] = DotProduct(playerBox.axis[0], axis[2]);

	AD[0] = DotProduct(playerBox.axis[0], D);

	absC[0][0] = (float)fabsf(C[0][0]);
	absC[0][1] = (float)fabsf(C[0][1]);
	absC[0][2] = (float)fabsf(C[0][2]);

	R = (float)fabsf(AD[0]);
	R1 = extent[0] * absC[0][0] + extent[1] * absC[0][1] + extent[2] * absC[0][2];
	R01 = playerBox.extent[0] + R1;
	if (R > R01) {
			return FALSE;
	}


	//A1
	C[1][0] = DotProduct(playerBox.axis[1], axis[0]);
	C[1][1] = DotProduct(playerBox.axis[1], axis[1]);
	C[1][2] = DotProduct(playerBox.axis[1], axis[2]);

	AD[1] = DotProduct(playerBox.axis[1], D);

	absC[1][0] = (float)fabsf(C[1][0]);
	absC[1][1] = (float)fabsf(C[1][1]);
	absC[1][2] = (float)fabsf(C[1][2]);

	R = (float)fabsf(AD[1]);
	R1 = extent[0] * absC[1][0] + extent[1] * absC[1][1] + extent[2] * absC[1][2];
	R01 = playerBox.extent[1] + R1;
	if (R > R01) {
		
		return FALSE;
	}


	//A2
	C[2][0] = DotProduct(playerBox.axis[2], axis[0]);
	C[2][1] = DotProduct(playerBox.axis[2], axis[1]);
	C[2][2] = DotProduct(playerBox.axis[2], axis[2]);

	AD[2] = DotProduct(playerBox.axis[2], D);

	absC[2][0] = (float)fabsf(C[2][0]);
	absC[2][1] = (float)fabsf(C[2][1]);
	absC[2][2] = (float)fabsf(C[2][2]);

	R = (float)fabsf(AD[2]);
	R1 = extent[0] * absC[2][0] + extent[1] * absC[2][1] + extent[2] * absC[2][2];
	R01 = playerBox.extent[2] + R1;
	if (R > R01) {
	
		return FALSE;
	}

	// axis 축과의 충돌체크
//B0
	R = (float)fabsf(DotProduct(axis[0], D));
	R0 = playerBox.extent[0] * absC[0][0] + playerBox.extent[1] * absC[1][0] + playerBox.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01) {

		return FALSE;
	}

	//B1
	R = (float)fabsf(DotProduct(axis[1], D));
	R0 = playerBox.extent[0] * absC[0][1] + playerBox.extent[1] * absC[1][1] + playerBox.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01) {

		return FALSE;
	}

	//B2
	R = (float)fabsf(DotProduct(axis[2], D));
	R0 = playerBox.extent[0] * absC[0][2] + playerBox.extent[1] * absC[1][2] + playerBox.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01) {
		return FALSE;
	}

	//axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	//A0xB0
	R = (float)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = playerBox.extent[1] * absC[2][0] + playerBox.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A0xB1
	R = (float)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = playerBox.extent[1] * absC[2][1] + playerBox.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A0xB2
	R = (float)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = playerBox.extent[1] * absC[2][2] + playerBox.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A1xB0
	R = (float)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = playerBox.extent[0] * absC[2][0] + playerBox.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A1xB1
	R = (float)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = playerBox.extent[0] * absC[2][1] + playerBox.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}
	//A1xB2
	R = (float)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = playerBox.extent[0] * absC[2][2] + playerBox.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A2xB0
	R = (float)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = playerBox.extent[0] * absC[1][0] + playerBox.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A2xB1
	R = (float)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = playerBox.extent[0] * absC[1][1] + playerBox.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}

	//A2xB2
	R = (float)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = playerBox.extent[0] * absC[1][2] + playerBox.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		return FALSE;
	}
	collideRet = TRUE;

	////충돌했어
	//Vector3 boxCenter, playerBoxCenter;
	//float centerDistanceX, centerDistanceZ;

	//// 충돌한 박스의 center 와의 거리를 계산
	//boxCenter.x = (center[0] < 0) ? (center[0]* - 1.0f) : center[0];
	//boxCenter.z = (center[2] < 0) ? (center[2]* - 1.0f) : center[2];
	//playerBoxCenter.x = (playerBox.center[0] < 0) ? ( playerBox.center[0] * - 1.0f) : 1.0f;
	//playerBoxCenter.z = (playerBox.center[2] < 0) ? ( playerBox.center[2] * - 1.0f) : 1.0f;
	//centerDistanceX = playerBoxCenter.x - boxCenter.x;
	//centerDistanceZ = playerBoxCenter.z - boxCenter.z;

	//centerDistanceX = (centerDistanceX < 0) ? (centerDistanceX * -1.0f) : centerDistanceX;
	//centerDistanceZ = (centerDistanceZ < 0) ? (centerDistanceZ * -1.0f) : centerDistanceZ;

	//if ( (extent[0] + playerBox.extent[0]) <= centerDistanceX)
	//{
	//	if (boxCenter.x < playerBox.center[0])
	//		collisionDirection = 1;
	//	else
	//		collisionDirection = 2;
	//}
	//else if ((extent[2] + playerBox.extent[2]) <= centerDistanceZ)
	//{
	//	if (boxCenter.z < playerBox.center[2])
	//		collisionDirection = 5;
	//	else
	//		collisionDirection = 6;
	//}
	//else
	//	collisionDirection = 0;


	return  TRUE;
}
//bool CBox::Intersection(const CBox& box0, int& collisionDirection)
//{
//
//	float dx = fabsf(center[0] - box0.center[0]);
//	float sumX = extent[0] + box0.extent[0];
//	if (dx > sumX) {
//		return false;
//	}
//
//	float dz = fabsf(center[2] - box0.center[2]);
//	float sumZ = extent[2] + box0.extent[2];
//	if (dz > sumZ) {
//		return false;
//	}
//
//	// 충돌됨, 충돌 방향 설정
//	collisionDirection = 0;  // 충돌 방향 초기화
//
//	// 충돌 방향 판단
//	if (dx > 0) {
//		collisionDirection |= 1;  // 왼쪽 충돌
//	}
//	else if (dx < 0) {
//		collisionDirection |= 2;  // 오른쪽 충돌
//	}
//
//	if (dz > 0) {
//		collisionDirection |= 3;  // 위쪽 충돌
//	}
//	else if (dz < 0) {
//		collisionDirection |= 4;  // 아래쪽 충돌
//	}
//
//	// 충돌됨
//	return true;
//}


bool CBox::Intersection2(const CBox& playerbox, int& collisionDirection, Vector3& collisionSlidingVector)
{
	float dx = fabsf(center[0] - playerbox.center[0]);
	float sumX = extent[0] + playerbox.extent[0];
	//float collisionDirection = 0;
	if (dx > sumX) {
		return false;
	}
	else {
		float collisionDepthX = sumX - dx; // x 충돌 면의 깊이

		float dz = fabsf(center[2] - playerbox.center[2]);
		float sumZ = extent[2] + playerbox.extent[2];
		if (dz > sumZ) {
			return false;
		}
		else {
			float collisionDepthZ = sumZ - dz; // z 충돌 면의 깊이

			// 충돌 면의 깊이를 이용하여 충돌 방향과 이동 벡터를 설정합니다.
			if (collisionDepthX < collisionDepthZ) {
				// x 축에서 충돌한 경우
				
				collisionDirection = 0; // 충돌 방향 설정 (적절한 값으로 변경)
				collisionSlidingVector = Vector3(collisionDepthX, 0.0f, 0.0f); // 이동 벡터 설정
			}
			else {
				// z 축에서 충돌한 경우
				collisionDirection = 1; // 충돌 방향 설정 (적절한 값으로 변경)
				collisionSlidingVector = Vector3(0.0f, 0.0f, collisionDepthZ); // 이동 벡터 설정
			}

			return true;
		}
	}
}


Vector3 CBox::CalculateSlidingVector(const CBox& playerBox, int collisionDirection, Vector3 playerVelocity)
{
	Vector3 slidingVector{ 0.0f, 0.0f, 0.0f };

	// 충돌 방향에 따라 충돌 법선 벡터를 계산합니다.
	Vector3 collisionNormal{ 0.0f, 0.0f, 0.0f };
	if (collisionDirection == 1) {
		collisionNormal.x = 1.0f;
	}
	else if (collisionDirection == 2) {
		collisionNormal.x = -1.0f;
	}
	else if (collisionDirection == 3) {
		collisionNormal.z = 1.0f;
	}
	else if (collisionDirection == 4) {
		collisionNormal.z = -1.0f;
	}

	// 플레이어 속도와 충돌 법선의 내적을 계산합니다.
	//float dotProduct = playerVelocity.Dot(collisionNormal);
	float dotProduct = playerVelocity.x * collisionNormal.x + playerVelocity.y * collisionNormal.y + playerVelocity.z * collisionNormal.z;

	// 슬라이딩 벡터를 계산합니다.
	slidingVector.x = playerVelocity.x - 2.0f * dotProduct * collisionNormal.x;
	slidingVector.z = playerVelocity.z - 2.0f * dotProduct * collisionNormal.z;

	return slidingVector;
}

Vector3 CBox::CalculateSlidingVector2(CBox& playerBox, Vector3 playerVelocity, Vector3 collisionNormal)
{
	Vector3 slidingVector;

	// 충돌한 두 객체의 상대 속도 계산
	Vector3 relativeVelocity = playerVelocity;


	collisionNormal.Normalize();

	// 충돌 방향에 따라 슬라이딩 벡터 계산
	slidingVector = relativeVelocity - relativeVelocity.ProjectOnVector(collisionNormal);

	return slidingVector;
}

//Vector3 CBox::CalculateSlidingVector3(CBox& playerBox, Vector3 playerVelocity, Vector3 collisionNormal)
//{
//
//}

Vector3 CBox::CalculateSliding(CBox& playerBox, Vector3 playerVelocity, int collisionDirection)
{
	Vector3 slidingVector;

	// 충돌한 두 객체의 상대 속도 계산
	Vector3 relativeVelocity;
	relativeVelocity = playerVelocity;

	// 충돌 법선 벡터 계산
	Vector3 collisionNormal;


	// collisionDirection에 따른 collisionNormal 설정
	if ( collisionDirection == 1 ) {
			collisionNormal = { 1, 0, 0 };
	}
	else if (collisionDirection == 2) {
		collisionNormal = { -1, 0, 0 };
	}
	else if ( collisionDirection == 3 ) {
			collisionNormal = {0, 1, 0};
	}
	else if (collisionDirection == 4) {
		collisionNormal = { 0, -1, 0 };
	}
	else if ( collisionDirection == 5 ) {
			collisionNormal = {0,  0,  1};
	}
	else if (collisionDirection == 6) {
		collisionNormal = { 0,  0,  -1};
	}


	collisionDirection = 0;
	collisionNormal.Normalize();
	// 슬라이딩 벡터 계산
	float dotProduct = relativeVelocity.Dot(collisionNormal);
	Vector3 scaledNormal = collisionNormal * dotProduct;
	slidingVector = relativeVelocity - scaledNormal;

	return slidingVector;
}

