#include "pch.h"
#include "CBox.h"


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
	R1 = extent[0] *10* absC[0][0] + extent[1] * 10 * absC[0][1] + extent[2] * 10 * absC[0][2];
	R01 = box0.extent[0] * 10 + R1;
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
	R1 = extent[0] * 10 * absC[1][0] + extent[1] * 10 * absC[1][1] + extent[2] * 10 * absC[1][2];
	R01 = box0.extent[1] * 10 + R1;
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
	R1 = extent[0] * 10 * absC[2][0] + extent[1] * 10 * absC[2][1] + extent[2] * 10 * absC[2][2];
	R01 = box0.extent[2] * 10 + R1;
	if (R > R01) {

		return false;
	}

	// axis 축과의 충돌체크
	// B0
	R = (float)fabsf(DotProduct(axis[0], D));
	R0 = box0.extent[0] * 10 * absC[0][0] + box0.extent[1] * 10 * absC[1][0] + box0.extent[2] * 10 * absC[2][0];
	R01 = R0 + extent[0] * 10;
	if (R > R01) {

		return false;
	}

	// B1
	R = (float)fabsf(DotProduct(axis[1], D));
	R0 = box0.extent[0] * 10 * absC[0][1] + box0.extent[1] * 10 * absC[1][1] + box0.extent[2] * 10 * absC[2][1];
	R01 = R0 + extent[1] * 10;
	if (R > R01) {

		return false;
	}

	// B2
	R = (float)fabsf(DotProduct(axis[2], D));
	R0 = box0.extent[0]  * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2]  * absC[2][2];
	R01 = R0 + extent[2] * 10;
	if (R > R01) {

		return false;
	}

	// axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	// A0xB0
	R = (float)fabsf(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1]  * absC[2][0] + box0.extent[2]  * absC[1][0];
	R1 = extent[1] * 10 * absC[0][2] + extent[2] * 10 * absC[0][1];
	R01 = R0 + R1;
	if (R > R01) {

		return false;
	}

	// A0xB1
	R = (float)fabsf(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1]  * absC[2][1] + box0.extent[2]  * absC[1][1];
	R1 = extent[0] * 10 * absC[0][2] + extent[2] * 10 * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {

		return false;
	}

	// A0xB2
	R = (float)fabsf(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1]  * absC[2][2] + box0.extent[2]  * absC[1][2];
	R1 = extent[0] * 10 * absC[0][1] + extent[1] * 10 * absC[0][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A1xB0
	R = (float)fabsf(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0]  * absC[2][0] + box0.extent[2]  * absC[0][0];
	R1 = extent[1] * 10 * absC[1][2] + extent[2] * 10 * absC[1][1];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A1xB1
	R = (float)fabsf(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0]  * absC[2][1] + box0.extent[2]  * absC[0][1];
	R1 = extent[0] * 10 * absC[1][2] + extent[2] * 10 * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A1xB2
	R = (float)fabsf(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0]  * absC[2][2] + box0.extent[2]  * absC[0][2];
	R1 = extent[0] * 10 * absC[1][1] + extent[1] * 10 * absC[1][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A2xB0
	R = (float)fabsf(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0]  * absC[1][0] + box0.extent[1]  * absC[0][0];
	R1 = extent[1] * 10 * absC[2][2] + extent[2] * 10 * absC[2][1];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A2xB1
	R = (float)fabsf(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0]  * absC[1][1] + box0.extent[1]  * absC[0][1];
	R1 = extent[0] * 10 * absC[2][2] + extent[2] * 10 * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// A2xB2
	R = (float)fabsf(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0]  * absC[1][2] + box0.extent[1]  * absC[0][2];
	R1 = extent[0] * 10 * absC[2][1] + extent[1] * 10 * absC[2][0];
	R01 = R0 + R1;
	if (R > R01) {
		return false;
	}

	// 모든 축과 외적 및 내적 검사를 통과한 경우, 두 상자는 교차합니다.
	return true;
}

bool CBox::Intersection2(const CBox& playerbox, int& collisionDirection)
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
			}
			else {
				// z 축에서 충돌한 경우
				collisionDirection = 1; // 충돌 방향 설정 (적절한 값으로 변경)
			}

			return true;
		}
	}
}


bool CBox::IntersectionRay(const CBox& RayBox)
{
	float dx = fabsf(center[0] - RayBox.center[0]);
	float sumX = extent[0] + RayBox.extent[0];

	if (dx > sumX) {
		return false;
	}
	else {
		float dy = fabsf(center[1] - RayBox.center[1]);
		float sumY = extent[1] + RayBox.extent[1];

		if (dy > sumY) {
			return false;
		}
		else {
			float dz = fabsf(center[2] - RayBox.center[2]);
			float sumZ = extent[2] + RayBox.extent[2];

			if (dz > sumZ) {
				return false;
			}
			else {
				return true;
			}
		}
	}
}