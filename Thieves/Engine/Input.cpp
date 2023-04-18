#include "pch.h"
#include "Input.h"
#include "Engine.h"
#include "Timer.h"

void Input::Init(HWND hwnd)
{
	_hwnd = hwnd;
	_states.resize(KEY_TYPE_COUNT, KEY_STATE::NONE);
}

void Input::Update()
{
	HWND hwnd = ::GetActiveWindow();
	_userID = L"";
	// �ڵ��� ���ٸ� ��� key�� KEY_STATE�� �ʱ�ȭ�ϰ� Update�Լ� ����
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;
		return;
	}

	// ����Ű�� ���¸� asciiKeys�� ���� (���н� Update()�Լ� ����)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};	
	if (::GetKeyboardState(asciiKeys) == false)
		return;
	
	// �Է��� key�� KEY_STATE�� ����
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		count += 1;
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			_userID = (char)key;
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
			{
				state = KEY_STATE::DOWN;
				//if (isgraph(key))
				//	_userID += key;
				// ������Ʈ â ��� �ڵ�
				/*{
					system("cls");
					cout << "UserID : ";
					wcout << INPUT->GetUserID();
					cout << endl;
				}*/
			}
		}
		else
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::UP;
				count = 0;
			}
			else
				state = KEY_STATE::NONE;
		}
	}

	// ���콺 �Է�
	{
		::GetCursorPos(&_mousePos);
		::ScreenToClient(GEngine->GetWindow().hwnd, &_mousePos);
	}

}