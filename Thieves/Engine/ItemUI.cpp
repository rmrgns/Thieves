#include "pch.h"
#include "ItemUI.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Transform.h"

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
	// if Player Item Empty
	// if get Gun Item
	if (this->GetGameObject()->GetName() == L"ItemBox_Gun")
	{
		Vec3 pos = GetTransform()->GetLocalPosition();
		pos.z -= 1.f;
		GetTransform()->SetLocalPosition(pos);
	}

	// if Player Item Gun
	// if use Gun Item
	if (this->GetGameObject()->GetName() == L"ItemBox_Gun")
	{
		Vec3 pos = GetTransform()->GetLocalPosition();
		pos.z += 1.f;
		GetTransform()->SetLocalPosition(pos);
	}

	// if Player Item Empty
	// if get Gun Item
	if (this->GetGameObject()->GetName() == L"ItemBox_Trap")
	{
		Vec3 pos = GetTransform()->GetLocalPosition();
		pos.z -= 1.f;
		GetTransform()->SetLocalPosition(pos);
	}

	// if Player Item Gun
	// if use Gun Item
	if (this->GetGameObject()->GetName() == L"ItemBox_Trap")
	{
		Vec3 pos = GetTransform()->GetLocalPosition();
		pos.z += 1.f;
		GetTransform()->SetLocalPosition(pos);
	}
}
