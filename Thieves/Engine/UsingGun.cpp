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
	// �÷��̾� ��ġ ��ǥ
	Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 vec = GetTransform()->GetLook();

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		// ������ �÷��̾� ��ġ�� ���� ������ ��

	}
}
