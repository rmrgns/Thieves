#include "pch.h"
#include "ray_casting.h"
#include "CBox.h"

Vector3 RayCasting::Shoot(Vector3 RayPos, Vector3 RayDir)
{
	CBox Raytemp;
	MapManager _map_manager;
	Vector3 collisionPoint;
	int MAXRANGE = 100;
	
	//center
	Raytemp.center[0] = RayPos.x;
	Raytemp.center[1] = 100.f;
	Raytemp.center[2] = RayPos.z;

	//extent
	Raytemp.extent[0] = 1.f;
	Raytemp.extent[1] = 1.f;
	Raytemp.extent[2] = 1.f;

	//right
	Raytemp.axis[0][0] = 1.f;
	Raytemp.axis[0][1] = 0.f;
	Raytemp.axis[0][2] = 0.f;
			
	//up
	Raytemp.axis[1][0] = 0.f;
	Raytemp.axis[1][1] = 1.f;
	Raytemp.axis[1][2] = 0.f;
		
	//look
	Raytemp.axis[2][0] = 0.f;
	Raytemp.axis[2][1] = 0.f;
	Raytemp.axis[2][2] = 1.f;

	//translation
	Raytemp.translation[0] = RayDir.x;
	Raytemp.translation[1] = RayDir.y;
	Raytemp.translation[2] = RayDir.z;

 	for (int i = 0 ; i < MAXRANGE ; i++)//충알 방향에 따라 xz축 +++
	{
		
		if (_map_manager.checkCollisionRay(Raytemp))
		{
			break;
		}
		Raytemp.center[0] = Raytemp.center[0] + RayDir.x;
		Raytemp.center[1] = Raytemp.center[1] + RayDir.y;
		Raytemp.center[2] = Raytemp.center[2] + RayDir.z;
	}	

	collisionPoint.x = Raytemp.center[0];
	collisionPoint.y = Raytemp.center[1];
	collisionPoint.z = Raytemp.center[2];
	// 충돌 X
	return collisionPoint;
}


Vector3 RayCasting::CheckSafe(Vector3 RayPos, Vector3 RayDir)
{
	CBox Raytemp;
	MapManager _map_manager;
	Vector3 collisionPoint;
	int MAXRANGE = 100;

	//center
	Raytemp.center[0] = RayPos.x;
	Raytemp.center[1] = 100.f;
	Raytemp.center[2] = RayPos.z;

	//extent
	Raytemp.extent[0] = 1.f;
	Raytemp.extent[1] = 1.f;
	Raytemp.extent[2] = 1.f;

	//right
	Raytemp.axis[0][0] = 1.f;
	Raytemp.axis[0][1] = 0.f;
	Raytemp.axis[0][2] = 0.f;

	//up
	Raytemp.axis[1][0] = 0.f;
	Raytemp.axis[1][1] = 1.f;
	Raytemp.axis[1][2] = 0.f;

	//look
	Raytemp.axis[2][0] = 0.f;
	Raytemp.axis[2][1] = 0.f;
	Raytemp.axis[2][2] = 1.f;

	//translation
	Raytemp.translation[0] = RayDir.x;
	Raytemp.translation[1] = RayDir.y;
	Raytemp.translation[2] = RayDir.z;

	for (int i = 0; i < MAXRANGE; i++)//충알 방향에 따라 xz축 +++
	{

		if (_map_manager.checkCollisionSafe(Raytemp))
		{
			return true;
		}
		Raytemp.center[0] = Raytemp.center[0] + RayDir.x;
		Raytemp.center[1] = Raytemp.center[1] + RayDir.y;
		Raytemp.center[2] = Raytemp.center[2] + RayDir.z;
	}

	collisionPoint.x = Raytemp.center[0];
	collisionPoint.y = Raytemp.center[1];
	collisionPoint.z = Raytemp.center[2];
	// 충돌 X
	return false;
}
