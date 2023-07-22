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
		Network::GetInst()->SendBullet(bullet_pos, lookVec, bullet_rotation);
		_bulletcheck = true;
		
	}
	

	if (_recv_bullet_pos == TRUE)
	{
		GetTransform()->SetLocalPosition(GetBulletPos());
		GetTransform()->SetLocalRotation(GetBulletRot());
		_recv_bullet_pos = FALSE;
	}
	//int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	//
	//if (myID != -1)
	//{
	//	bullet_pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
	//	bullet_rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
	//}
	//
	//Vec3 temp;
	//
	//GetTransform()->SetLocalPosition(Vec3((bullet_pos.x + temp.x) / 2.0f, (bullet_pos.y + 125.f + temp.y) / 2.0f, (bullet_pos.z + temp.z) / 2.0f));
	//
	//GetTransform()->SetLocalPosition(Vec3(bullet_pos.x, bullet_pos.y, bullet_pos.z));
	//transform->SetLocalRotation(GET_SINGLE(SceneManager)->GetLookVec());
	//GetTransform()->SetLocalRotation(bullet_rotation);

}


void UsingGun::DrawBullet()
{
	// 총알 궤적 그리기
	GetTransform()->SetLocalPosition(GetBulletPos());
	GetTransform()->SetLocalRotation(GetBulletRot());

	// 시간 체크 시작
	_bulletcheck = true;
}

void UsingGun::RecvBullet(Vec3 bullet_start_pos, Vec3 bullet_end_pos, Vec3 bullet_rot_pos)
{
	Vec3 b_point = { (bullet_start_pos.x + bullet_end_pos.x) / 2.0f, (bullet_start_pos.y + 125.f + bullet_end_pos.y) / 2.0f, (bullet_start_pos.z + bullet_end_pos.z) / 2.0f };
	
	SetBulletPos(b_point);
	SetBulletRot(bullet_rot_pos);

	//GetTransform()->SetLocalPosition(b_point);
	//GetTransform()->SetLocalRotation(GET_SINGLE(SceneManager)->GetLookVec());
	//_bulletcheck = true;
	//
	//SetBulletRot(GET_SINGLE(SceneManager)->GetLookVec());
	//DrawBullet();
}

