#include "pch.h"
#include "ItemScript.h"
#include "server/main/network.h"
#include "MapItem.h"
#include "Transform.h"

void ItemScript::Update()
{
	auto Item = Network::GetInst()->GetItemObjMap().find(m_ID)->second;

	switch (Item->GetState())
	{
	case ITEM_STATE::IT_NONE:
		this->GetTransform()->SetLocalPosition(Vec3(0.f, -4000.f, 0.f));
		break;
	case ITEM_STATE::IT_SPAWN:
		this->GetTransform()->SetLocalPosition(Vec3(Item->GetPosition().x, 100.f, Item->GetPosition().z));
		break;
	case ITEM_STATE::IT_OCCUPIED:
		this->GetTransform()->SetLocalPosition(Item->GetPosition());
		break;
	case ITEM_STATE::IT_SET:
		if (Item->GetItemType() == ITEM_NUM_TRAP)
		{
			this->GetTransform()->SetLocalPosition(Item->GetPosition());
		}
		break;
	default:
		break;
	}

}
