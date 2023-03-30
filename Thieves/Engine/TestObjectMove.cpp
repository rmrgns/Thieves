#include "pch.h"
#include "TestObjectMove.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"
#include "thieves_server/thieves_packet/thieves_packet_manager.h"

TestObjectMove::TestObjectMove()
{
}

TestObjectMove::~TestObjectMove()
{
}

void TestObjectMove::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();

	if (INPUT->GetButton(KEY_TYPE::UP))
		pos += GetTransform()->GetLook() * _speed * DELTA_TIME;
	
	if (INPUT->GetButton(KEY_TYPE::DOWN))
		pos -= GetTransform()->GetLook() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::LEFT))
		pos -= GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::RIGHT))
		pos += GetTransform()->GetRight() * _speed * DELTA_TIME;

	if (INPUT->GetButton(KEY_TYPE::R))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::Y))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::V))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::N))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DELTA_TIME * 1.f;
		GetTransform()->SetLocalRotation(rotation);
	}

	// Test packet
	if (INPUT->GetButton(KEY_TYPE::L))
	{

	}

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		const POINT& point = INPUT->GetMousePos();
		INPUT->SetPrevMousePos(point);
	}
	else if (INPUT->GetButton(KEY_TYPE::LBUTTON))
	{
		const POINT& point = INPUT->GetMousePos();	// 현재 마우스좌표를 point변수에 저장
		// rotation 예시
		/*{
			if (point.x > INPUT->GetPrevMousePos().x)
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();
				rotation.y -= DELTA_TIME * 10.f;
				GetTransform()->SetLocalRotation(rotation);
			}
			else if (point.x < INPUT->GetPrevMousePos().x)
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();
				rotation.y += DELTA_TIME * 10.f;
				GetTransform()->SetLocalRotation(rotation);
			}

			if (point.y > INPUT->GetPrevMousePos().y)
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();
				rotation.x -= DELTA_TIME * 10.f;
				GetTransform()->SetLocalRotation(rotation);
			}
			else if (point.y < INPUT->GetPrevMousePos().y)
			{
				Vec3 rotation = GetTransform()->GetLocalRotation();
				rotation.x += DELTA_TIME * 10.f;
				GetTransform()->SetLocalRotation(rotation);
			}
		}*/

		// translation 예시
		{
			if (point.x > INPUT->GetPrevMousePos().x)
			{
				pos -= GetTransform()->GetRight() * (point.x - INPUT->GetPrevMousePos().x) * _objectTranslationSpeed * DELTA_TIME;
			}
			else if (point.x < INPUT->GetPrevMousePos().x)
			{
				pos += GetTransform()->GetRight() * (INPUT->GetPrevMousePos().x - point.x) * _objectTranslationSpeed * DELTA_TIME;
			}

			if (point.y > INPUT->GetPrevMousePos().y)
			{
				pos += GetTransform()->GetUp() * (point.y - INPUT->GetPrevMousePos().y) * _objectTranslationSpeed * DELTA_TIME;
			}
			else if (point.y < INPUT->GetPrevMousePos().y)
			{
				pos -= GetTransform()->GetUp() * (INPUT->GetPrevMousePos().y - point.y) * _objectTranslationSpeed * DELTA_TIME;
			}
		}
		INPUT->SetPrevMousePos(point);	// Input class의 _prevMousePos에 point의 좌표를 저장
	}

	GetTransform()->SetLocalPosition(pos);
}