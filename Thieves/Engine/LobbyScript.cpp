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

	if (point.x > 472 && point.x < 472 + 383)
	{
		if (point.y > 178 && point.y < 178 + 92)
		{
			Network::GetInst()->SendEnterRoom(0);
		}
		else if (point.y > 178 + 134 && point.y < 178 + 134 + 92)
		{
			Network::GetInst()->SendEnterRoom(2);
		}
		else if (point.y > 178 + 134 + 134 && point.y < 178 + 134 + 134 + 92)
		{
			Network::GetInst()->SendEnterRoom(4);
		}
		else if (point.y > 178 + 134 + 134 + 134 && point.y < 178 + 134 + 134 + 134 + 92)
		{
			Network::GetInst()->SendEnterRoom(6);
		}
	}
	else if (point.x > 908 && point.x < 908 + 383)
	{
		if (point.y > 178 && point.y < 178 + 92)
		{
			Network::GetInst()->SendEnterRoom(1);
		}
		else if (point.y > 178 + 134 && point.y < 178 + 134 + 92)
		{
			Network::GetInst()->SendEnterRoom(3);
		}
		else if (point.y > 178 + 134 + 134 && point.y < 178 + 134 + 134 + 92)
		{
			Network::GetInst()->SendEnterRoom(5);
		}
		else if (point.y > 178 + 134 + 134 + 134 && point.y < 178 + 134 + 134 + 134 + 92)
		{
			Network::GetInst()->SendEnterRoom(7);
		}
	}
	if (point.x > 248 && point.x < 410)
	{
		if (point.y > 378 && point.y < 480)
		{
			GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
			GEngine->SetChangeScene(L"Login");
			
		}
		else if (point.y > 492 && point.y < 575)
		{
			Network::GetInst()->SendEnterRoom(-1);
			
		}
	}
}
