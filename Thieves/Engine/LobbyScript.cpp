#include "pch.h"
#include "LobbyScript.h"
#include "Input.h"
#include "server/main/network.h"

#include "SceneManager.h"
#include "Engine.h"

void LobbyScript::Update()
{
	if (INPUT->GetButtonDown(KEY_TYPE::KET_0)) {
		Network::GetInst()->SendEnterRoom(0);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_1)) {
		Network::GetInst()->SendEnterRoom(1);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_2)) {
		Network::GetInst()->SendEnterRoom(2);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_3)) {
		Network::GetInst()->SendEnterRoom(3);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_4)) {
		Network::GetInst()->SendEnterRoom(4);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_5)) {
		Network::GetInst()->SendEnterRoom(5);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_6)) {
		Network::GetInst()->SendEnterRoom(6);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_7)) {
		Network::GetInst()->SendEnterRoom(7);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::KEY_8)) {
		Network::GetInst()->SendEnterRoom(8);
	}
	else if (INPUT->GetButtonDown(KEY_TYPE::O)) {
		Network::GetInst()->SendEnterRoom(-1);
	}

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		ClickRobbyButton();
	}
}

void LobbyScript::ClickRobbyButton()
{
	POINT point = {};
	GetCursorPos(&point);
	ScreenToClient(GEngine->GetWindow().hwnd, &point);

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);

	if (point.x > width * 330.f / 1600.f && point.x < width * 860.f / 1600.f)
	{
		if (point.y > height * 115.f / 900.f && point.y < height * 230.f / 900.f)
		{
			Network::GetInst()->SendEnterRoom(0);
		}
		else if (point.y > height * (115.f + 172.f) / 900.f && point.y < height * (230.f + 172.f) / 900.f)
		{
			Network::GetInst()->SendEnterRoom(2);
		}
		else if (point.y > height * (115.f + 172.f + 172.f) / 900.f && point.y < height * (230.f + 172.f + 172.f) / 900.f)
		{
			Network::GetInst()->SendEnterRoom(4);
		}
		else if (point.y > height * (115.f + 172.f + 172.f + 172.f) / 900.f && point.y < height * (230.f + 172.f + 172.f + 172.f) / 900.f)
		{
			Network::GetInst()->SendEnterRoom(6);
		}
	}
	else if (point.x > width * 963.f / 1600.f && point.x < width * 1490.f / 1600.f)
	{
		if (point.y > height * 115.f / 900.f && point.y < height * 230.f / 900.f)
		{
			Network::GetInst()->SendEnterRoom(1);
		}
		else if (point.y > height * (115.f + 172.f) / 900.f && point.y < height * (230.f + 172.f) / 900.f)
		{
			Network::GetInst()->SendEnterRoom(3);
		}
		else if (point.y > height * (115.f + 172.f + 172.f) / 900.f && point.y < height * (230.f + 172.f + 172.f) / 900.f)
		{
			Network::GetInst()->SendEnterRoom(5);
		}
		else if (point.y > height * (115.f + 172.f + 172.f + 172.f) / 900.f && point.y < height * (230.f + 172.f + 172.f + 172.f) / 900.f)
		{
			Network::GetInst()->SendEnterRoom(7);
		}
	}

	if (point.x > width * 25.f / 1600.f && point.x < width * 250.f / 1600.f)
	{
		if (point.y > height * 305.f / 900.f && point.y < height * 380.f / 900.f)
		{
			GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
			GEngine->SetChangeScene(L"Login");
			
		}
		else if (point.y > height * 413.f / 900.f && point.y < height * 488.f / 900.f)
		{
			Network::GetInst()->SendEnterRoom(-1);
			
		}
	}
}
