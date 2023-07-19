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
		// �÷��̾� ��ġ ��ǥ
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

		// �������� �÷��̾� ��ġ�� ���� �޴� ��
		//dir.x = GET_SINGLE(SceneManager)->GetLookVec().x * 100.f;
		//dir.z = GET_SINGLE(SceneManager)->GetLookVec().z * 100.f;
		//dir.y = GET_SINGLE(SceneManager)->GetLookVec().y * 100.f;

		//�Ѿ� ���� ��ġ
		//Network::GetInst()->SendBullet(pos, dir);

		// �÷��̾� ��ġ�� ���ʹ� playerInput ��ũ��Ʈ���� ����
		// pos ���� = (�� ��ġ + ��� ��ġ) / 2 -> x,z�� �ش�
		
		
		// �Ѿ� ���� �׸���
		GetTransform()->SetLocalPosition(pos);
		GetTransform()->SetLocalRotation(rotation);

		// �ð� üũ ����
		_bulletcheck = true;

	}
	

}
