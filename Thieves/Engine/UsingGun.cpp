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
	// 플레이어 위치 좌표
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	Vec3 pos(0.0f, 0.0f, 0.0f);
	Vec3 rotation(0.0f, 0.0f, 0.0f);
	if (myID != -1)
	{
		pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
		rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
	}
	pos.x += GetTransform()->GetLook().x * 100.f;
	pos.z += GetTransform()->GetLook().z * 100.f;
	pos.y += 100.f;

	

	if (INPUT->GetButtonDown(KEY_TYPE::LBUTTON) && _bulletcheck == false)
	{
		// 서버에 플레이어 위치와 벡터 보내는 곳
		Network::Network::GetInst()->SendBullet(pos, rotation);




		// 총알 궤적 그리기
		shared_ptr<Scene> scene = GET_SINGLE(SceneManager)->GetLoadProgressScene();
#pragma region Object
		{
			shared_ptr<GameObject> obj = make_shared<GameObject>();
			obj->SetName(L"BulletLine");
			obj->AddComponent(make_shared<Transform>());
			//obj->AddComponent(make_shared<SphereCollider>());
			obj->GetTransform()->SetLocalScale(Vec3(2.f, 2.f, 1000.f));
			obj->GetTransform()->SetLocalPosition(pos);
			obj->GetTransform()->SetLocalRotation(rotation);
			obj->SetStatic(true);


			shared_ptr<MeshRenderer> meshRenderer = make_shared<MeshRenderer>();
			{
				shared_ptr<Mesh> sphereMesh = GET_SINGLE(Resources)->LoadCubeMesh();
				meshRenderer->SetMesh(sphereMesh);
			}
			{
				shared_ptr<Material> material = GET_SINGLE(Resources)->Get<Material>(L"GameObject");
				meshRenderer->SetMaterial(material->Clone());
			}
			//dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetRadius(0.5f);
			//dynamic_pointer_cast<SphereCollider>(obj->GetCollider())->SetCenter(Vec3(0.f, 0.f, 0.f));
			obj->AddComponent(meshRenderer);
			scene->AddGameObject(obj);
		}
#pragma endregion
		// 시간 체크 시작
		_bulletcheck = true;

	}
	if (_bulletcheck)
	{
		_count += DELTA_TIME;
		if (_count > 1.0f)
		{
			for (auto& gameObject : GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects())
			{
				if (gameObject->GetName() == L"BulletLine")
				{
					GET_SINGLE(SceneManager)->GetActiveScene()->RemoveGameObject(gameObject);
					_count = 0.f;
					_bulletcheck = false;
				}
			}
		}
	}

}
