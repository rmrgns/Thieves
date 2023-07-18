#include "pch.h"
#include "server/main/network.h"
#include "RoomScene.h"

void RoomScene::GetRoomDataFromNetwork()
{
	isRecvDataEnd = false;
	ClearPlayerData();

	Network::GetInst()->SendRequestInRoomData();

	while (!isRecvDataEnd) {
		Sleep(0);
	}
}
