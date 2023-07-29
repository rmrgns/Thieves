#include "pch.h"
#include "LoginScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Scene.h"
#include "server/main/network.h"
LoginScript::LoginScript()
{
}

LoginScript::~LoginScript()
{
}

void LoginScript::LateUpdate()
{
	
	if (INPUT->GetButtonDown(KEY_TYPE::O))
	{
		if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOGIN)
		{
			Network::GetInst()->SendSignInPacket();
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::P))
	{
		INPUT->InitUserId();
		INPUT->InitUserPassword();
		GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
		GEngine->SetChangeScene(L"Login");
	}

	if (INPUT->GetButtonDown(KEY_TYPE::ESC))
	{
		PostQuitMessage(0);
	}

	if (INPUT->GetButtonUp(KEY_TYPE::ENTER))
	{
		
		if (INPUT->GetUserID() == INPUT->GetServerID() && INPUT->GetUserPassword() == L"TEST")
		{
			GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
			GEngine->SetChangeScene(L"Game");
		}
		if (INPUT->GetCheckIDorPassword() == false)
		{
			INPUT->InitUserId();
			INPUT->InitUserPassword();
		}

	}

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		ClickLoginButton();
	}
}

void LoginScript::ClickLoginButton()
{
	POINT point = {};
	GetCursorPos(&point);
	ScreenToClient(GEngine->GetWindow().hwnd, &point);

	float width = static_cast<float>(GEngine->GetWindow().width);
	float height = static_cast<float>(GEngine->GetWindow().height);
	
	if (point.x > width * 970 / 1600 && point.x < width * 1365 / 1600)
	{
		if (point.y > height * 585 / 900 && point.y < height * 665 / 900)
		{
			if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOGIN)
			{
				Network::GetInst()->SendSignInPacket();
			}
		}
	}
	
}
