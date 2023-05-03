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
		// ������ Ÿ���� ���� ������ �׳� ��������.
		return;
	}
		break;
	case NetworkType::PLAYER:
	{
		// �ڱ� �ڽ��� ��쿡��, ���� �����͵��� �������� �� ��ġ�� ���� �־�� ��. ��, �� ��� �� ���ʹ� �������� �ʾƵ� ��.
		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		
	}
		break;
	case NetworkType::OTHER_PLAYER:
	{
		// �ڱ� �ڽ��� �ƴ� �÷��̾� �� ��쿡��, ���� �����͵��� �������� �ش��ϴ� �÷��̾��� ��ġ�� �����־�� ��.

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		this->GetTransform()->SetLocalRotation(myData->GetRotation());
	}

		break;
	case NetworkType::NPC:
	{
		// NPC�� ��쿡��, ���� �����͵��� �������� �ش��ϴ� �÷��̾��� ��ġ�� �����־�� ��.

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		this->GetTransform()->SetLocalRotation(myData->GetRotation());
	}
		break;
	default:
		break;
	}
}
