#include "pch.h"
#include "collision_detection.h"

int CollisionDetection::BoxBoxIntersectionTest(const CBox& box0, const CBox& box1)
{
	// compute difference of box centers,D=C1-C0
	float D[3] =
	{
	 box1.center[0] - box0.center[0],
	 box1.center[1] - box0.center[1],
	 box1.center[2] - box0.center[2]
	};

	float C[3][3];    //matrix C=A^T B,c_{ij}=Dot(A_i,B_j)
	float absC[3][3]; //|c_{ij}|
	float AD[3];      //Dot(A_i,D)
	float R0, R1, R;    //interval radii and distance between centers
	float R01;        //=R0+R1

	//A0
	C[0][0] = DotProduct(box0.axis[0], box1.axis[0]);
	C[0][1] = DotProduct(box0.axis[0], box1.axis[1]);
	C[0][2] = DotProduct(box0.axis[0], box1.axis[2]);
	AD[0] = DotProduct(box0.axis[0], D);
	absC[0][0] = (float)fc_fabs(C[0][0]);
	absC[0][1] = (float)fc_fabs(C[0][1]);
	absC[0][2] = (float)fc_fabs(C[0][2]);
	R = (float)fc_fabs(AD[0]);
	R1 = box1.extent[0] * absC[0][0] + box1.extent[1] * absC[0][1] + box1.extent[2] * absC[0][2];
	R01 = box0.extent[0] + R1;
	if (R > R01)return 0;

	//A1
	C[1][0] = DotProduct(box0.axis[1], box1.axis[0]);
	C[1][1] = DotProduct(box0.axis[1], box1.axis[1]);
	C[1][2] = DotProduct(box0.axis[1], box1.axis[2]);
	AD[1] = DotProduct(box0.axis[1], D);
	absC[1][0] = (float)fc_fabs(C[1][0]);
	absC[1][1] = (float)fc_fabs(C[1][1]);
	absC[1][2] = (float)fc_fabs(C[1][2]);
	R = (float)fc_fabs(AD[1]);
	R1 = box1.extent[0] * absC[1][0] + box1.extent[1] * absC[1][1] + box1.extent[2] * absC[1][2];
	R01 = box0.extent[1] + R1;
	if (R > R01)return 0;

	//A2
	C[2][0] = DotProduct(box0.axis[2], box1.axis[0]);
	C[2][1] = DotProduct(box0.axis[2], box1.axis[1]);
	C[2][2] = DotProduct(box0.axis[2], box1.axis[2]);
	AD[2] = DotProduct(box0.axis[2], D);
	absC[2][0] = (float)fc_fabs(C[2][0]);
	absC[2][1] = (float)fc_fabs(C[2][1]);
	absC[2][2] = (float)fc_fabs(C[2][2]);
	R = (float)fc_fabs(AD[2]);
	R1 = box1.extent[0] * absC[2][0] + box1.extent[1] * absC[2][1] + box1.extent[2] * absC[2][2];
	R01 = box0.extent[2] + R1;
	if (R > R01)return 0;

	//B0
	R = (float)fc_fabs(DotProduct(box1.axis[0], D));
	R0 = box0.extent[0] * absC[0][0] + box0.extent[1] * absC[1][0] + box0.extent[2] * absC[2][0];
	R01 = R0 + box1.extent[0];
	if (R > R01)return 0;

	//B1
	R = (float)fc_fabs(DotProduct(box1.axis[1], D));
	R0 = box0.extent[0] * absC[0][1] + box0.extent[1] * absC[1][1] + box0.extent[2] * absC[2][1];
	R01 = R0 + box1.extent[1];
	if (R > R01)return 0;

	//B2
	R = (float)fc_fabs(DotProduct(box1.axis[2], D));
	R0 = box0.extent[0] * absC[0][2] + box0.extent[1] * absC[1][2] + box0.extent[2] * absC[2][2];
	R01 = R0 + box1.extent[2];
	if (R > R01)return 0;

	//A0xB0
	R = (float)fc_fabs(AD[2] * C[1][0] - AD[1] * C[2][0]);
	R0 = box0.extent[1] * absC[2][0] + box0.extent[2] * absC[1][0];
	R1 = box1.extent[1] * absC[0][2] + box1.extent[2] * absC[0][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A0xB1
	R = (float)fc_fabs(AD[2] * C[1][1] - AD[1] * C[2][1]);
	R0 = box0.extent[1] * absC[2][1] + box0.extent[2] * absC[1][1];
	R1 = box1.extent[0] * absC[0][2] + box1.extent[2] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A0xB2
	R = (float)fc_fabs(AD[2] * C[1][2] - AD[1] * C[2][2]);
	R0 = box0.extent[1] * absC[2][2] + box0.extent[2] * absC[1][2];
	R1 = box1.extent[0] * absC[0][1] + box1.extent[1] * absC[0][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB0
	R = (float)fc_fabs(AD[0] * C[2][0] - AD[2] * C[0][0]);
	R0 = box0.extent[0] * absC[2][0] + box0.extent[2] * absC[0][0];
	R1 = box1.extent[1] * absC[1][2] + box1.extent[2] * absC[1][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB1
	R = (float)fc_fabs(AD[0] * C[2][1] - AD[2] * C[0][1]);
	R0 = box0.extent[0] * absC[2][1] + box0.extent[2] * absC[0][1];
	R1 = box1.extent[0] * absC[1][2] + box1.extent[2] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A1xB2
	R = (float)fc_fabs(AD[0] * C[2][2] - AD[2] * C[0][2]);
	R0 = box0.extent[0] * absC[2][2] + box0.extent[2] * absC[0][2];
	R1 = box1.extent[0] * absC[1][1] + box1.extent[1] * absC[1][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB0
	R = (float)fc_fabs(AD[1] * C[0][0] - AD[0] * C[1][0]);
	R0 = box0.extent[0] * absC[1][0] + box0.extent[1] * absC[0][0];
	R1 = box1.extent[1] * absC[2][2] + box1.extent[2] * absC[2][1];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB1
	R = (float)fc_fabs(AD[1] * C[0][1] - AD[0] * C[1][1]);
	R0 = box0.extent[0] * absC[1][1] + box0.extent[1] * absC[0][1];
	R1 = box1.extent[0] * absC[2][2] + box1.extent[2] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	//A2xB2
	R = (float)fc_fabs(AD[1] * C[0][2] - AD[0] * C[1][2]);
	R0 = box0.extent[0] * absC[1][2] + box0.extent[1] * absC[0][2];
	R1 = box1.extent[0] * absC[2][1] + box1.extent[1] * absC[2][0];
	R01 = R0 + R1;
	if (R > R01)return 0;

	return 1;
}