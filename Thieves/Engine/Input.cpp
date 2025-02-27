#include "pch.h"
#include "Input.h"
#include "Engine.h"
#include "SceneManager.h"

void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void Input::Update()
{
	HWND hwnd = ::GetActiveWindow();

	// 핸들이 없다면 모든 key의 KEY_STATE를 초기화하고 Update함수 종료
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;
		return;
	}

	// 입력한 key의 KEY_STATE값 변경
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		if (::GetAsyncKeyState(key) & 0x8000)
		{
			KEY_STATE& state = _states[key];
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
			{
				state = KEY_STATE::DOWN;
				if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOGIN)
				{
					if (_checkIDorPassword == false)
					{
						if (key == VK_RETURN)
						{
							_checkIDorPassword = true;
						}
						else
							_userID += key;
					}
					else
					{
						if (key == VK_RETURN)
						{
							_checkIDorPassword = false;
						}
						else
							_userPassword += key;
					}
				}
			}
		}
		else
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::UP;
				//_userID += (char)key;
			}
			else
				state = KEY_STATE::NONE;
		}
	}

	// 마우스 입력
	{
		::GetCursorPos(&_mousePos);
		::ScreenToClient(GEngine->GetWindow().hwnd, &_mousePos);
	}
}