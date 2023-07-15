#pragma once
#include "pch.h"

class RayCasting
{
	RayCasting();
	~RayCasting();
	// �÷��̾� center ��ġ
	float playerX, playerY;

	// �÷��̾� ���⺤��
	float playerDirX, playerDirY;

	// �÷��̾� ī�޶����
	float playerPlaneX, playerPlaneY;

	// screen Width Height
	float screenW, screenH;		// w/2, h/2 ȭ���߾� ����

	float cameraX;	// 2 * screenW / float(screenW) - 1;

	//������ ���⺤��
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