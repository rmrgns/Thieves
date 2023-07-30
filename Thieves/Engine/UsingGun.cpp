#include "pch.h"
#include "UsingGun.h"
#include "Transform.h"
#include "Input.h"
#include "GameObject.h"
#include "MeshRenderer.h"
#include "Resources.h"
#include "Scene.h"
#include "SceneManager.h"
#include "InGameScene.h"
#include "Timer.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/thieves_server/thieves_send_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"
UsingGun::UsingGun()
{
}

UsingGun::~UsingGun()
{
}
void UsingGun::LateUpdate()
{

	if (_bulletcheck)
	{
		_count += DELTA_TIME;
		if (_count > 1.0f)
		{
			GetTransform()->SetLocalPosition(Vec3(-100.f, -100.f, -100.f));
			_count = 0.f;
			_bulletcheck = false;
		}
		return;
	}


	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON) && _bulletcheck == false)
	{
		// 플레이어 위치 좌표
		int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();

		if (myID != -1)
		{
			bullet_pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
			bullet_rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
		}

		bullet_pos.x += GET_SINGLE(SceneManager)->GetLookVec().x * 100.f;
		bullet_pos.z += GET_SINGLE(SceneManager)->GetLookVec().z * 100.f;
		bullet_pos.y += 100.f;

		Vec3 lookVec = GET_SINGLE(SceneManager)->GetLookVec();

		// 서버로 전송
		Network::GetInst()->SendBullet(bullet_pos, bullet_rotation);
	}
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();

	if (myID != -1)
	{
		bullet_pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
		bullet_rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
	}

	Vec3 temp;

	GetTransform()->SetLocalPosition(Vec3((bullet_pos.x + temp.x) / 2.0f, (bullet_pos.y + 125.f + temp.y) / 2.0f, (bullet_pos.z + temp.z) / 2.0f));
	
	//GetTransform()->SetLocalPosition(Vec3(bullet_pos.x, bullet_pos.y, bullet_pos.z));
	//transform->SetLocalRotation(GET_SINGLE(SceneManager)->GetLookVec());
	GetTransform()->SetLocalRotation(bullet_rotation);
}


void UsingGun::DrawBullet()
{
	// 총알 궤적 그리기
	GetTransform()->SetLocalPosition(GetBulletPos());
	GetTransform()->SetLocalRotation(GetBulletRot());

	// 시간 체크 시작
	_bulletcheck = true;
}

void UsingGun::RecvBullet(Vec3 pos, Vec3 Rot)
{
	SetBulletPos(pos);
	SetBulletRot(Rot);
	DrawBullet();
}

