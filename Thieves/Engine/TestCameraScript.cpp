#include "pch.h"
#include "TestCameraScript.h"
#include "Transform.h"
#include "Camera.h"
#include "GameObject.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Physics.h"

TestCameraScript::TestCameraScript()
{
}

TestCameraScript::~TestCameraScript()
{
}

void TestCameraScript::LateUpdate()
{
	Vec3 pos = GetTransform()->GetLocalPosition();
	
	if (INPUT->GetButton(KEY_TYPE::W) || INPUT->GetButton(KEY_TYPE::S) || INPUT->GetButton(KEY_TYPE::A) || INPUT->GetButton(KEY_TYPE::D))
	{
		if (INPUT->GetButton(KEY_TYPE::W))
		{
			AccelerateLook();
			pos += GetTransform()->GetLook() * (_speed + _vel.x) * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::S))
		{
			AccelerateLook();
			pos -= GetTransform()->GetLook() * (_speed + _vel.x) * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::A))
		{
			AccelerateRight();
			pos -= GetTransform()->GetRight() * (_speed + _vel.z) * DELTA_TIME;
		}
		if (INPUT->GetButton(KEY_TYPE::D))
		{
			AccelerateRight();
			pos += GetTransform()->GetRight() * (_speed + _vel.z) * DELTA_TIME;
		}
	}

	if (INPUT->GetButtonUp(KEY_TYPE::W) ||
		INPUT->GetButtonUp(KEY_TYPE::S) ||
		INPUT->GetButtonUp(KEY_TYPE::A) ||
		INPUT->GetButtonUp(KEY_TYPE::D))
	{
		if (INPUT->GetButtonUp(KEY_TYPE::W))
		{
			_forceDirection.x = 0.f;
			_vel.x = 0.f;
		}
		if (INPUT->GetButtonUp(KEY_TYPE::S))
		{
			_forceDirection.x = 0.f;
			_vel.x = 0.f;
		}
		if (INPUT->GetButtonUp(KEY_TYPE::A))
		{
			_forceDirection.z = 0.f;
			_vel.z = 0.f;
		}
		if (INPUT->GetButtonUp(KEY_TYPE::D))
		{
			_forceDirection.z = 0.f;
			_vel.z = 0.f;
		}
	}

	if (INPUT->GetButton(KEY_TYPE::Q))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::E))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.x -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::Z))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y += DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	if (INPUT->GetButton(KEY_TYPE::C))
	{
		Vec3 rotation = GetTransform()->GetLocalRotation();
		rotation.y -= DELTA_TIME * 0.5f;
		GetTransform()->SetLocalRotation(rotation);
	}

	// picking 코드
	/*if (INPUT->GetButtonDown(KEY_TYPE::RBUTTON))
	{
		const POINT& pos = INPUT->GetMousePos();
		GET_SINGLE(SceneManager)->Pick(pos.x, pos.y);
	}*/

	//// mouse translation 코드
	//{
	//	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON))
	//	{
	//		const POINT& point = INPUT->GetMousePos();
	//		INPUT->SetPrevMousePos(point);
	//	}
	//	else if (INPUT->GetButton(KEY_TYPE::LBUTTON))
	//	{
	//		const POINT& point = INPUT->GetMousePos();	// 현재 마우스좌표를 point변수에 저장
	//		// translation 예시
	//		{
	//			if (point.x > INPUT->GetPrevMousePos().x)
	//			{
	//				pos -= GetTransform()->GetRight() * (point.x - INPUT->GetPrevMousePos().x) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//			else if (point.x < INPUT->GetPrevMousePos().x)
	//			{
	//				//pos += GetTransform()->GetRight() * (point.x - INPUT->GetPrevMousePos().x) * _mouseTranslationSpeed * DELTA_TIME;
	//				pos += GetTransform()->GetRight() * (INPUT->GetPrevMousePos().x - point.x) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//			if (point.y > INPUT->GetPrevMousePos().y)
	//			{
	//				pos += GetTransform()->GetUp() * (point.y - INPUT->GetPrevMousePos().y) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//			else if (point.y < INPUT->GetPrevMousePos().y)
	//			{
	//				pos -= GetTransform()->GetUp() * (INPUT->GetPrevMousePos().y - point.y) * _mouseTranslationSpeed * DELTA_TIME;
	//			}
	//		}
	//		INPUT->SetPrevMousePos(point);	// Input class의 _prevMousePos에 point의 좌표를 저장
	//	}
	//}
	
	//if (INPUT->GetButton(KEY_TYPE::RBUTTON))
	//{
	//	const POINT& point = INPUT->GetMousePos();	// 현재 마우스좌표를 point변수에 저장

	//	// rotation 예시
	//	{
	//		if (point.x > 0 && point.x < GEngine->GetWindow().width - 15)
	//		{
	//			if (point.x > INPUT->GetPrevMousePos().x)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.y += DELTA_TIME * (point.x - INPUT->GetPrevMousePos().x) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//			else if (point.x < INPUT->GetPrevMousePos().x)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.y -= DELTA_TIME * (INPUT->GetPrevMousePos().x - point.x) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//		}

	//		if (point.y > 0 && point.y < GEngine->GetWindow().height - 40)
	//		{
	//			if (point.y > INPUT->GetPrevMousePos().y)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.x += DELTA_TIME * (point.y - INPUT->GetPrevMousePos().y) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//			else if (point.y < INPUT->GetPrevMousePos().y)
	//			{
	//				Vec3 rotation = GetTransform()->GetLocalRotation();
	//				rotation.x -= DELTA_TIME * (INPUT->GetPrevMousePos().y - point.y) * _mouseRotateSpeed;
	//				GetTransform()->SetLocalRotation(rotation);
	//			}
	//		}}
	//	INPUT->SetPrevMousePos(point);	// Input class의 _prevMousePos에 point의 좌표를 저장
	//}

	GetTransform()->SetLocalPosition(pos);
}

void TestCameraScript::AccelerateLook()
{
	_forceDirection.x += 1.f;
	_vel.x += _forceDirection.x * DELTA_TIME;

	//float normalForce = 9.8f;
	//float fric = m_fricCoef * normalForce;

	//float mag = sqrtf(_vel.x * _vel.x);
	//if (mag > FLT_EPSILON)
	//{
	//	float velDir = _vel.x / mag;
	//	velDir *= -1.f;
	//	//velDir *= fric;
	//	//velDir /= m_mass;
	//	float resultVel = 0.f;
	//	resultVel = _vel.x + velDir;

	//	if (resultVel * _vel.x < 0.f)
	//	{
	//		_vel.x = 0.f;
	//	}
	//	else
	//	{
	//		_vel.x += velDir;
	//	}
	//}
	//else
	//{

	//}
}

void TestCameraScript::AccelerateRight()
{
	_forceDirection.z += 1.f;
	_vel.z += _forceDirection.z * DELTA_TIME;

	//float normalForce = 9.8f;
	//float fric = m_fricCoef * normalForce;

	//float mag = sqrtf(_vel.z * _vel.z);
	//if (mag > FLT_EPSILON)
	//{
	//	float velDir = _vel.z / mag;
	//	velDir *= -1.f;
	//	//velDir *= fric;
	//	//velDir /= m_mass;
	//	float resultVel = 0.f;
	//	resultVel = _vel.z + velDir;

	//	if (resultVel * _vel.z < 0.f)
	//	{
	//		_vel.z = 0.f;
	//	}
	//	else
	//	{
	//		_vel.z += velDir;
	//	}
	//}
	//else
	//{

	//}
}