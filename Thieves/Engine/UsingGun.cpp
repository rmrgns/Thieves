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
		Vec3 pos(0.0f, 0.0f, 0.0f);
		Vec3 rotation(0.0f, 0.0f, 0.0f);
		Vec3 dir();
		if (myID != -1)
		{
			pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
			rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
		}
		
		pos.x += GET_SINGLE(SceneManager)->GetLookVec().x * 100.f;
		//pos.y += GET_SINGLE(SceneManager)->GetLookVec().y * 100.f;
		pos.z += GET_SINGLE(SceneManager)->GetLookVec().z * 100.f;
		pos.y += 120.f;

		// 서버에서 플레이어 위치와 벡터 받는 곳
		//dir.x = GET_SINGLE(SceneManager)->GetLookVec().x * 100.f;
		//dir.z = GET_SINGLE(SceneManager)->GetLookVec().z * 100.f;
		//dir.y = GET_SINGLE(SceneManager)->GetLookVec().y * 100.f;

		//총알 시작 위치
		//Network::GetInst()->SendBullet(pos, dir);

		// 플레이어 위치와 벡터는 playerInput 스크립트에서 전송
		// pos 공식 = (내 위치 + 상대 위치) / 2 -> x,z만 해당
		
		
		// 총알 궤적 그리기
		GetTransform()->SetLocalPosition(pos);
		GetTransform()->SetLocalRotation(rotation);

		// 시간 체크 시작
		_bulletcheck = true;

	}
	

}
