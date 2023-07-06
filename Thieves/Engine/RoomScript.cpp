#include "pch.h"
#include "Input.h"
#include "RoomScript.h"
#include "SceneManager.h"
#include "RoomScene.h"
#include "Engine.h"
#include "server/main/network.h"

void RoomScript::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::R)) {
		int playerId = Network::GetInst()->GetPacketManager()->GetID();

		shared_ptr<RoomScene> rScene =
			static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		
		bool isPlayerReady = rScene->GetPlayerReady(playerId);

		//rScene->SetPlayerReady(playerId, !isPlayerReady);

		if (rScene->GetPlayerReady(playerId)) {
			Network::GetInst()->SendReady();
		}
		else {
			Network::GetInst()->SendCancleReady();
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::Q)) {
		int playerId = Network::GetInst()->GetPacketManager()->GetID();

		shared_ptr<RoomScene> rScene =
			static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());

		Network::GetInst()->SendLeaveRoom();

		GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
		GEngine->SetChangeScene(L"Lobby");

	}

	if (INPUT->GetButton(KEY_TYPE::S)) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		if (rScene->GetRoomMasterId() == Network::GetInst()->GetPacketManager()->GetID())
		{
			Network::GetInst()->SendStartPacket();
		}
	}
}
