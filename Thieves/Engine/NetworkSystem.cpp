#include "pch.h"
#include "NetworkSystem.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "Animator.h"
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
		// ���⼭ Ŭ���̾�Ʈ�� ��ǲ ó���� ���� �ϴ� ���� ���� ����.
		// ��Ŷ�� ������ �־�� �ϱ� ������.

		// ó�� ��Ŷ ����
		// inputData�� ��Ʈ �������� �����.

		char inputData = 0;

		if (INPUT->GetButton(KEY_TYPE::W))
		{
			inputData = inputData | 8; // 1000;
		}

		if (INPUT->GetButton(KEY_TYPE::A))
		{
			inputData = inputData | 4; // 0100
		}

		if (INPUT->GetButton(KEY_TYPE::S))
		{
			inputData = inputData | 2; // 0010
		}

		if (INPUT->GetButton(KEY_TYPE::D))
		{
			inputData = inputData | 1; // 0001
		}

		Network::GetInst()->SendMovePacket(inputData, GetTransform()->GetLocalPosition(),
			GetTransform()->GetLook(), DELTA_TIME, GetAnimator()->GetCurrentClipIndex());
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
		if (this->GetAnimator()->GetCurrentClipIndex() != myData->GetActionType() && (myData->GetActionType() < 16 && myData->GetActionType() >= 0))
		{
			this->GetAnimator()->Play(myData->GetActionType());
		}
	}

		break;
	case NetworkType::NPC:
	{
		// NPC�� ��쿡��, ���� �����͵��� �������� �ش��ϴ� �÷��̾��� ��ġ�� �����־�� ��.

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		this->GetTransform()->SetLocalPosition(myData->GetPosition());
		this->GetTransform()->SetLocalRotation(myData->GetRotation());
		if (this->GetAnimator()->GetCurrentClipIndex() != myData->GetActionType() && (myData->GetActionType() > 16 || myData->GetActionType() < 0))
		{
			this->GetAnimator()->Play(myData->GetActionType());
		}
	}
		break;
	default:
		break;
	}
}
