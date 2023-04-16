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

LoginScript::LoginScript()
{
}

LoginScript::~LoginScript()
{
}

void LoginScript::LateUpdate()
{
	if (INPUT->GetButton(KEY_TYPE::O))
	{
		GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
		GEngine->SetChangeScene(L"Game");
	}

	if (INPUT->GetButton(KEY_TYPE::P))
	{
		INPUT->InitUserId();
		GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
		GEngine->SetChangeScene(L"Login");
	}

	if (INPUT->GetButton(KEY_TYPE::ESC))
	{
		PostQuitMessage(0);
	}

	if (INPUT->GetButtonUp(KEY_TYPE::ENTER))
	{
		
		if (INPUT->GetUserID() == INPUT->GetServerID())
		{
			GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
			GEngine->SetChangeScene(L"Game");
		}
		INPUT->InitUserId();
	}
	
}