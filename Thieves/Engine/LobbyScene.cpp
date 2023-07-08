#include "pch.h"
#include "server/main/network.h"
#include "server/thieves_server/thieves_send_manager.h"
#include "LobbyScene.h"

void LobbyScene::GetRoomsDataFromNetwork()
{
	isRecvDataEnd = false;
	ClearRoomData();

	Network::GetInst()->SendRequestRoomsData();

	while (!isRecvDataEnd) {
		Sleep(0);
	}
}
