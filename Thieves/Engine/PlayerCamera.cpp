#include "pch.h"
#include "PlayerCamera.h"
#include "SceneManager.h"
#include "Engine.h"
#include "Input.h"
#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
PlayerCamera::PlayerCamera()
{
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::LateUpdate()
{
	//Vec3 pos = GetTransform()->GetLocalPosition();
	Vec3 pos = GET_SINGLE(SceneManager)->GetPlayerPosition();
	pos.y += 125.f;
	// 카메라 마우스제어
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		CameraRotation();
	}
	GetTransform()->SetLocalPosition(pos);
}

void PlayerCamera::CameraRotation()
{
	const POINT& point = INPUT->GetMousePos();	// 현재 마우스좌표를 point변수에 저장
	POINT WindowCenter = GEngine->GetWindowCenter();
	POINT ClientCenter = GEngine->GetClientCenter();

	// 중앙에서 마우스가 이동한 거리의 값을 float값으로 변경
	float dx = static_cast<float>(point.x - ClientCenter.x);	
	float dy = static_cast<float>(point.y - ClientCenter.y);

	// 커서를 화면 중앙으로 이동
	SetCursorPos(WindowCenter.x, WindowCenter.y);

	// rotation
	Vec3 rotation = GetTransform()->GetLocalRotation();
	rotation.x += DELTA_TIME * dy * _mouseRotateSpeed;
	rotation.y += DELTA_TIME * dx * _mouseRotateSpeed;

	// 상하 회전 제한
	if (rotation.x < -LIMIT_ROTATION)
	{
		rotation.x = -LIMIT_ROTATION;
	}
	if (rotation.x > LIMIT_ROTATION)
	{
		rotation.x = LIMIT_ROTATION;
	}

	// 카메라 캐릭터 rotation 일치 -> 1인칭
	GET_SINGLE(SceneManager)->SetPlayerRotation(rotation);
	
	GetTransform()->SetLocalRotation(rotation);
}
