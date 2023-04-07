#include "pch.h"
#include "Input.h"
#include "Engine.h"

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

	// 입력한 키가 KEY_TYPE에 있는지 확인 (실패시 Update()함수 종료)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// 입력한 key의 KEY_STATE값 변경
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// 키가 눌려 있으면 true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// 이전 프레임에 키를 누른 상태라면 PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
			{
				state = KEY_STATE::DOWN;
				if (isgraph(key))
					_userID += key;
				// 프롬프트 창 출력 코드
				{
					system("cls");
					cout << "UserID : ";
					wcout << INPUT->GetUserID();
					cout << endl;
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