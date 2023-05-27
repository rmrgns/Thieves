#include "pch.h"
#include "LobbyScript.h"
#include "Input.h"
#include "server/main/network.h"

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
	

}
