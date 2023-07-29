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

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		ClickRoomButton();
	}
}

void RoomScript::ClickRoomButton()
{
	POINT point = {};
	GetCursorPos(&point);
	ScreenToClient(GEngine->GetWindow().hwnd, &point);

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (point.x > width * 25.f / 1600.f && point.x < width * 250.f / 1600.f)
	{
		if (point.y > height * 305.f / 900.f && point.y < height * 360.f / 900.f)
		{
			int playerId = Network::GetInst()->GetPacketManager()->GetID();

			shared_ptr<RoomScene> rScene =
				static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());

			Network::GetInst()->SendLeaveRoom();

			GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
			GEngine->SetChangeScene(L"Lobby");
			//Network::GetInst()->SendSignInPacket();
		}
		else if (point.y > height * 430.f / 900.f && point.y < height * 480.f / 900.f)
		{
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
		else if (point.y > height * 550.f / 900.f && point.y < height * 595.f / 900.f)
		{
			shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
			if (rScene->GetRoomMasterId() == Network::GetInst()->GetPacketManager()->GetID())
			{
				Network::GetInst()->SendStartPacket();
			}
		}
	}
}
