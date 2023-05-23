#include "pch.h"
#include "CBox.h"

bool CBox::BoxBoxIntersectionTest(const CBox& box0, const CBox& box1)
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
	float R0, R1, R;    //interval radii and distance between centers
	float R01;        //=R0+R1

	//A0
	C[0][0] = DotProduct(box0.axis[0], axis[0]);
	C[0][1] = DotProduct(box0.axis[0], axis[1]);
	C[0][2] = DotProduct(box0.axis[0], axis[2]);
	AD[0] = DotProduct(box0.axis[0], D);
	absC[0][0] = (float)fc_fabs(C[0][0]);
	absC[0][1] = (float)fc_fabs(C[0][1]);
	absC[0][2] = (float)fc_fabs(C[0][2]);
	R = (float)fc_fabs(AD[0]);
	R1 = extent[0] * absC[0][0] + extent[1] * absC[0][1] + extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;
	if (R > R01)return 0;

	//A1
	C[1][0] = DotProduct(box0.axis[1], axis[0]);
	C[1][1] = DotProduct(box0.axis[1], axis[1]);
	C[1][2] = DotProduct(box0.axis[1], axis[2]);
	AD[1] = DotProduct(box0.axis[1], D);
	absC[1][0] = (float)fc_fabs(C[1][0]);
	absC[1][1] = (float)fc_fabs(C[1][1]);
	absC[1][2] = (float)fc_fabs(C[1][2]);
	R = (float)fc_fabs(AD[1]);
	R1 = extent[0] * absC[1][0] + extent[1] * absC[1][1] + extent[2] * absC[1][2];
	R01 = box0.extent[1] + R1;
	if (R > R01)return 0;

	//A2
	C[2][0] = DotProduct(box0.axis[2], axis[0]);
	C[2][1] = DotProduct(box0.axis[2], axis[1]);
	C[2][2] = DotProduct(box0.axis[2], axis[2]);
	AD[2] = DotProduct(box0.axis[2], D);
	absC[2][0] = (float)fc_fabs(C[2][0]);
	absC[2][1] = (float)fc_fabs(C[2][1]);
	absC[2][2] = (float)fc_fabs(C[2][2]);
	R = (float)fc_fabs(AD[2]);
	R1 = extent[0] * absC[2][0] + extent[1] * absC[2][1] + extent[2] * absC[2][2];
	R01 = box0.extent[2] + R1;
	if (R > R01)return 0;

	//B0
	R = (float)fc_fabs(DotProduct(axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01)return 0;

	//B1
	R = (float)fc_fabs(DotProduct(axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01)return 0;

	//B2
	R = (float)fc_fabs(DotProduct(axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01)return 0;

	//A0xB0
	R = (float)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A0xB1
	R = (float)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A0xB2
	R = (float)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB0
	R = (float)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB1
	R = (float)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB2
	R = (float)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB0
	R = (float)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB1
	R = (float)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB2
	R = (float)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	return 1;
}

bool CBox::BoxBoxIntersection(const CBox& box0)
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
	float R0, R1, R;    //interval radii and distance between centers
	float R01;        //=R0+R1


	// box0.axis 축과의 충돌체크
	// 
	//A0
	// 1
	C[0][0] = DotProduct(box0.axis[0],axis[0]);
	C[0][1] = DotProduct(box0.axis[0],axis[1]);
	C[0][2] = DotProduct(box0.axis[0],axis[2]);

	AD[0] = DotProduct(box0.axis[0], D);
	absC[0][0] = (float)fc_fabs(C[0][0]);
	absC[0][1] = (float)fc_fabs(C[0][1]);
	absC[0][2] = (float)fc_fabs(C[0][2]);
	R = (float)fc_fabs(AD[0]);
	R1 = extent[0] * absC[0][0] + extent[1] * absC[0][1] + extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;
	if (R > R01)return false;

	//A1
	C[1][0] = DotProduct(box0.axis[1], axis[0]);
	C[1][1] = DotProduct(box0.axis[1], axis[1]);
	C[1][2] = DotProduct(box0.axis[1], axis[2]);
	AD[1] = DotProduct(box0.axis[1], D);
	absC[1][0] = (float)fc_fabs(C[1][0]);
	absC[1][1] = (float)fc_fabs(C[1][1]);
	absC[1][2] = (float)fc_fabs(C[1][2]);
	R = (float)fc_fabs(AD[1]);
	R1 = extent[0] * absC[1][0] + extent[1] * absC[1][1] + extent[2] * absC[1][2];
	R01 = box0.extent[1] + R1;
	if (R > R01)return false;

	//A2
	C[2][0] = DotProduct(box0.axis[2], axis[0]);
	C[2][1] = DotProduct(box0.axis[2], axis[1]);
	C[2][2] = DotProduct(box0.axis[2], axis[2]);
	AD[2] = DotProduct(box0.axis[2], D);
	absC[2][0] = (float)fc_fabs(C[2][0]);
	absC[2][1] = (float)fc_fabs(C[2][1]);
	absC[2][2] = (float)fc_fabs(C[2][2]);
	R = (float)fc_fabs(AD[2]);
	R1 = extent[0] * absC[2][0] + extent[1] * absC[2][1] + extent[2] * absC[2][2];
	R01 = box0.extent[2] + R1;
	if (R > R01)return false;


	// axis 축과의 충돌체크
	//B0
	R = (float)fc_fabs(DotProduct(axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01)return false;

	//B1
	R = (float)fc_fabs(DotProduct(axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01)return false;

	//B2
	R = (float)fc_fabs(DotProduct(axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01)return false;


	//axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	//A0xB0
	R = (float)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A0xB1
	R = (float)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A0xB2
	R = (float)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB0
	R = (float)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB1
	R = (float)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB2
	R = (float)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB0
	R = (float)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB1
	R = (float)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB2
	R = (float)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;


	// 충돌 안했어
	return true;
}

bool CBox::BoxBoxIntersection2(const CBox& box0)
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
	float R0, R1, R;    //interval radii and distance between centers
	float R01;        //=R0+R1


	// box0.axis 축과의 충돌체크
	// 
	//A0
	// 1
	C[0][0] = DotProduct(box0.axis[0], axis[0]);
	C[0][1] = DotProduct(box0.axis[0], axis[1]);
	C[0][2] = DotProduct(box0.axis[0], axis[2]);

	AD[0] = DotProduct(box0.axis[0], D);

	absC[0][0] = (float)fc_fabs(C[0][0]);
	absC[0][1] = (float)fc_fabs(C[0][1]);
	absC[0][2] = (float)fc_fabs(C[0][2]);

	R = (float)fc_fabs(AD[0]);

	R1 = extent[0] * absC[0][0] + extent[1] * absC[0][1] + extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;

	if (R > R01)return false;

	//A1
	C[1][0] = DotProduct(box0.axis[1], axis[0]);
	C[1][1] = DotProduct(box0.axis[1], axis[1]);
	C[1][2] = DotProduct(box0.axis[1], axis[2]);
	AD[1] = DotProduct(box0.axis[1], D);
	absC[1][0] = (float)fc_fabs(C[1][0]);
	absC[1][1] = (float)fc_fabs(C[1][1]);
	absC[1][2] = (float)fc_fabs(C[1][2]);
	R = (float)fc_fabs(AD[1]);
	R1 = extent[0] * absC[1][0] + extent[1] * absC[1][1] + extent[2] * absC[1][2];
	R01 = box0.extent[1] + R1;
	if (R > R01)return false;

	//A2
	C[2][0] = DotProduct(box0.axis[2], axis[0]);
	C[2][1] = DotProduct(box0.axis[2], axis[1]);
	C[2][2] = DotProduct(box0.axis[2], axis[2]);
	AD[2] = DotProduct(box0.axis[2], D);
	absC[2][0] = (float)fc_fabs(C[2][0]);
	absC[2][1] = (float)fc_fabs(C[2][1]);
	absC[2][2] = (float)fc_fabs(C[2][2]);
	R = (float)fc_fabs(AD[2]);
	R1 = extent[0] * absC[2][0] + extent[1] * absC[2][1] + extent[2] * absC[2][2];
	R01 = box0.extent[2] + R1;
	if (R > R01)return false;


	// axis 축과의 충돌체크
	//B0
	R = (float)fc_fabs(DotProduct(axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + extent[0];
	if (R > R01)return false;

	//B1
	R = (float)fc_fabs(DotProduct(axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + extent[1];
	if (R > R01)return false;

	//B2
	R = (float)fc_fabs(DotProduct(axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + extent[2];
	if (R > R01)return false;


	//axis 벡터들 간의 외적과 내적을 사용하여 충돌 체크
	//A0xB0
	R = (float)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = extent[1] * absC[0][2] + extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A0xB1
	R = (float)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = extent[0] * absC[0][2] + extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A0xB2
	R = (float)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = extent[0] * absC[0][1] + extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB0
	R = (float)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = extent[1] * absC[1][2] + extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB1
	R = (float)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = extent[0] * absC[1][2] + extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A1xB2
	R = (float)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = extent[0] * absC[1][1] + extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB0
	R = (float)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = extent[1] * absC[2][2] + extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB1
	R = (float)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = extent[0] * absC[2][2] + extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;

	//A2xB2
	R = (float)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = extent[0] * absC[2][1] + extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return false;


	// 충돌 했어
	return true;
}

