#include "pch.h"
#include "NetworkSystem.h"
#include "Transform.h"
// Server
#include "server/main/network.h"
#include "server/ptr.h"
#include "server/main/packet_manager.h"
#include "server/main/network_move_object.h"

NetworkSystem::NetworkSystem() : Component(COMPONENT_TYPE::NETWORKOBJECT)
{

}

NetworkSystem::~NetworkSystem()
{
}

void NetworkSystem::Update()
{
	switch (_nType)
	{
	case NetworkType::NONE:
	{
		// 정해진 타입이 없을 때에는 그냥 리턴해줌.
		return;
	}
		break;
	case NetworkType::PLAYER:
	{
		// 자기 자신일 경우에는, 받은 데이터들을 바탕으로 내 위치를 정해 주어야 함. 단, 이 경우 룩 벡터는 적용하지 않아도 됨.
		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		
	}
		break;
	case NetworkType::OTHER_PLAYER:
	{
		// 자기 자신이 아닌 플레이어 인 경우에는, 받은 데이터들을 바탕으로 해당하는 플레이어의 위치를 정해주어야 함.

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		this->GetTransform()->SetLocalRotation(myData->GetRotation());
	}

		break;
	case NetworkType::NPC:
	{
		// NPC의 경우에는, 받은 데이터들을 바탕으로 해당하는 플레이어의 위치를 정해주어야 함.

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		this->GetTransform()->SetLocalRotation(myData->GetRotation());
	}
		break;
	default:
		break;
	}
}
