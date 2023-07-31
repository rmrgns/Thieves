#include "pch.h"
#include "ItemUI.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "InGameScene.h"
#include "Transform.h"
#include "ParticleSystem.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"

ItemUI::ItemUI()
{
}

ItemUI::~ItemUI()
{
}

void ItemUI::LateUpdate()
{
	auto iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
	auto& map = Network::GetInst()->GetPacketManager()->GetItemObjMap();



	if (iScene->GetItemNum() == -1)
	{
		Vec3 pos = GetTransform()->GetLocalPosition();
		pos.z = 501.f;
		GetTransform()->SetLocalPosition(pos);
	}
	else
	{
		for (auto& GameObject : iScene->GetGameObjects())
		{
			if (GameObject->GetName() == L"ParticleGetItem")
			{
				GameObject->GetParticleSystem()->UseParticle(true);
				break;
			}
		}

		if (map.find(iScene->GetItemNum())->second->GetItemType() == ITEM_NUM_GUN)
		{
			if (this->GetGameObject()->GetName() == L"ItemBox_Gun")
			{
				Vec3 pos = GetTransform()->GetLocalPosition();
				pos.z = 499.f;
				GetTransform()->SetLocalPosition(pos);
			}
		}
		else if (map.find(iScene->GetItemNum())->second->GetItemType() == ITEM_NUM_TRAP)
		{
			if (this->GetGameObject()->GetName() == L"ItemBox_Trap")
			{
				Vec3 pos = GetTransform()->GetLocalPosition();
				pos.z = 499.f;
				GetTransform()->SetLocalPosition(pos);
			}
		}
		else
		{

		}
	}
	//// if Player Item Empty
	//// if get Gun Item
	//if (this->GetGameObject()->GetName() == L"ItemBox_Gun")
	//{
	//	Vec3 pos = GetTransform()->GetLocalPosition();
	//	pos.z -= 1.f;
	//	GetTransform()->SetLocalPosition(pos);
	//}

	//// if Player Item Gun
	//// if use Gun Item
	//if (this->GetGameObject()->GetName() == L"ItemBox_Gun")
	//{
	//	Vec3 pos = GetTransform()->GetLocalPosition();
	//	pos.z += 1.f;
	//	GetTransform()->SetLocalPosition(pos);
	//}

	//// if Player Item Empty
	//// if get Gun Item
	//if (this->GetGameObject()->GetName() == L"ItemBox_Trap")
	//{
	//	Vec3 pos = GetTransform()->GetLocalPosition();
	//	pos.z -= 1.f;
	//	GetTransform()->SetLocalPosition(pos);
	//}

	//// if Player Item Gun
	//// if use Gun Item
	//if (this->GetGameObject()->GetName() == L"ItemBox_Trap")
	//{
	//	Vec3 pos = GetTransform()->GetLocalPosition();
	//	pos.z += 1.f;
	//	GetTransform()->SetLocalPosition(pos);
	//}
}
