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
	if (_start == false) 
	{
		if (INPUT->GetButtonDown(KEY_TYPE::O))
		{
			_start = true;
			
			GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
			GEngine->SetChangeScene(L"Game");

			Network::GetInst()->SendSignInPacket();
			Network::GetInst()->SendStartPacket();
			
		}
	}

	if (INPUT->GetButtonDown(KEY_TYPE::P))
	{
		INPUT->InitUserId();
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

}