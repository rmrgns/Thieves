#pragma once

enum class KEY_TYPE
{
	UP = VK_UP,
	DOWN = VK_DOWN,
	LEFT = VK_LEFT,
	RIGHT = VK_RIGHT,

	W = 'W',
	A = 'A',
	S = 'S',
	D = 'D',

	Q = 'Q',
	E = 'E',
	Z = 'Z',
	C = 'C',

	R = 'R',
	Y = 'Y',
	V = 'V',
	N = 'N',

	G = 'G',

	O = 'O',	// temp : GameScene전환
	P = 'P',	// temp : LoginScene전환
	L = 'L',
	F = 'F',
	KET_0 = '0',
	KEY_1 = '1',
	KEY_2 = '2',
	KEY_3 = '3',
	KEY_4 = '4',
	KEY_5 = '5',
	KEY_6 = '6',
	KEY_7 = '7',
	KEY_8 = '8',
	KEY_9 = '9',

	// packet test
	//L = 'L',

	ESC = VK_ESCAPE,
	ENTER = VK_RETURN,

	SPACE = VK_SPACE,

	LBUTTON = VK_LBUTTON,
	RBUTTON = VK_RBUTTON,
	SHF = VK_SHIFT,
};

enum class KEY_STATE
{
	NONE,
	PRESS,
	DOWN,
	UP,
	END
};

enum
{
	KEY_TYPE_COUNT = static_cast<int32>(UINT8_MAX + 1),
	KEY_STATE_COUNT = static_cast<int32>(KEY_STATE::END),
};

class Input
{
	DECLARE_SINGLE(Input);

public:
	void Init(HWND hwnd);
	void Update();

	// 누르고 있을 때
	bool GetButton(KEY_TYPE key) { return GetState(key) == KEY_STATE::PRESS; }
	// 맨 처음 눌렀을 때
	bool GetButtonDown(KEY_TYPE key) { return GetState(key) == KEY_STATE::DOWN; }
	// 맨 처음 눌렀다 뗐을 때_w
	bool GetButtonUp(KEY_TYPE key) { return GetState(key) == KEY_STATE::UP; }
	
	const POINT& GetMousePos() { return _mousePos; }
	const POINT& GetPrevMousePos() { return _prevMousePos; }
	void SetPrevMousePos(POINT prevMousePos) { _prevMousePos = prevMousePos; }
	
	wstring GetUserID() const { return _userID; }
	void InitUserId() { _userID = L""; }
	wstring GetUserPassword() const { return _userPassword; }
	void InitUserPassword() { _userPassword = L""; }

	bool GetCheckIDorPassword() const { return _checkIDorPassword; }

	wstring GetServerID() const { return _serverID; }
private:
	inline KEY_STATE GetState(KEY_TYPE key) { return _states[static_cast<uint8>(key)]; }
	
private:
	HWND _hwnd;
	vector<KEY_STATE> _states;
	vector<KEY_STATE> _mouseState;
	POINT _mousePos = {};
	POINT _prevMousePos = {};
	wstring _userID;
	wstring _userPassword;
	wstring _serverID = L"TEST";
	bool _checkIDorPassword = false;
};

