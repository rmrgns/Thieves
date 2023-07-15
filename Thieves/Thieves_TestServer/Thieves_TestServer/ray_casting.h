#pragma once
#include "pch.h"

class RayCasting
{
	RayCasting();
	~RayCasting();
	// 플레이어 center 위치
	float playerX, playerY;

	// 플레이어 방향벡터
	float playerDirX, playerDirY;

	// 플레이어 카메라평면
	float playerPlaneX, playerPlaneY;

	// screen Width Height
	float screenW, screenH;		// w/2, h/2 화면중앙 고정

	float cameraX;	// 2 * screenW / float(screenW) - 1;

	//광선의 방향벡터
	float rayDirX;		// playerDirX + playerPlaneX * cameraX;
	float rayDirY;		// playerDirY + playerPlaneY * cameraX;

	float sideDistX, sideDistY;
	float deltaDistX = std::abs(1 / rayDirX);
	float deltaDistY = std::abs(1 / rayDirY);
	float perpWallDist;

	int stepX;
	int stepY;

	int hit = 0; 
	int side; 

	float calcularateDeltaDist(float, float);

};