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

	// �ڵ��� ���ٸ� ��� key�� KEY_STATE�� �ʱ�ȭ�ϰ� Update�Լ� ����
	if (_hwnd != hwnd)
	{
		for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
			_states[key] = KEY_STATE::NONE;
		return;
	}

	// �Է��� Ű�� KEY_TYPE�� �ִ��� Ȯ�� (���н� Update()�Լ� ����)
	BYTE asciiKeys[KEY_TYPE_COUNT] = {};
	if (::GetKeyboardState(asciiKeys) == false)
		return;

	// �Է��� key�� KEY_STATE�� ����
	for (uint32 key = 0; key < KEY_TYPE_COUNT; key++)
	{
		// Ű�� ���� ������ true
		if (asciiKeys[key] & 0x80)
		{
			KEY_STATE& state = _states[key];

			// ���� �����ӿ� Ű�� ���� ���¶�� PRESS
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
				state = KEY_STATE::PRESS;
			else
			{
				state = KEY_STATE::DOWN;
				if (isgraph(key))
					_userID += key;
				// ������Ʈ â ��� �ڵ�
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

			// ���� �����ӿ� Ű�� ���� ���¶�� UP
			if (state == KEY_STATE::PRESS || state == KEY_STATE::DOWN)
			{
				state = KEY_STATE::UP;
				//_userID += (char)key;
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