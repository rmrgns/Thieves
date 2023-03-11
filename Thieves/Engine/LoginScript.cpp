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
		GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
		GEngine->SetChangeScene(L"Login");
	}

	if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{

		//GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
		//GEngine->SetChangeScene(L"Game");
		//const POINT& pos = INPUT->GetMousePos();
		//GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
	}

}