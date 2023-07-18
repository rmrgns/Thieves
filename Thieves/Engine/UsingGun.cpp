#include "pch.h"
#include "UsingGun.h"
#include "Transform.h"
#include "Input.h"

UsingGun::UsingGun()
{
}

UsingGun::~UsingGun()
{
}

void UsingGun::LateUpdate()
{
	// 플레이어 위치 좌표
	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 vec = GetTransform()->GetLook();

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		// 서버에 플레이어 위치와 벡터 보내는 곳

	}
}
