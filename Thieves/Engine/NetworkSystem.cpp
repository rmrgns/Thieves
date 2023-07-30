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
#include "SceneManager.h"
#include "InGameScene.h"

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
		// -> ��°������ 0, 0, 0�� �����ǹǷ� ���⼭ �Ⱥ��̰� ó���� �ϴ°��� ���ƺ���.
		this->GetTransform()->SetLocalPosition(Vec3(0.f, -300.f, 0.f));
		this->GetTransform()->SetLocalRotation(Vec3(0.f, 0.f, 0.f));
		return;
	}
		break;
	case NetworkType::PLAYER:
	{
		auto iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		if (!iScene->GetIsTimerStart()) break;

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		// ���⼭ Ŭ���̾�Ʈ�� ��ǲ ó���� ���� �ϴ� ���� ���� ����.
		// ��Ŷ�� ������ �־�� �ϱ� ������.

		// ó�� ��Ŷ ����
		// inputData�� ��Ʈ �������� �����.

		char inputData = 0;

		if (INPUT->GetButton(KEY_TYPE::W))
		{
			inputData = inputData | 8; // 00001000;
		}

		if (INPUT->GetButton(KEY_TYPE::A))
		{
			inputData = inputData | 4; // 00000100
		}

		if (INPUT->GetButton(KEY_TYPE::S))
		{
			inputData = inputData | 2; // 00000010
		}

		if (INPUT->GetButton(KEY_TYPE::D))
		{
			inputData = inputData | 1; // 00000001
		}

		if (INPUT->GetButton(KEY_TYPE::SPACE))
		{
			inputData = inputData | 16; // 00010000
		}

		Network::GetInst()->SendMovePacket(inputData, GetTransform()->GetLocalPosition(),
			GetTransform()->GetLook(), DELTA_TIME, GetAnimator()->GetCurrentClipIndex());
		// �ڱ� �ڽ��� ��쿡��, ���� �����͵��� �������� �� ��ġ�� ���� �־�� ��. ��, �� ��� �� ���ʹ� �������� �ʾƵ� ��.
		

		this->GetTransform()->SetLocalPosition(myData->GetPosition());

		
	}
		break;
	case NetworkType::OTHER_PLAYER:
	{
		// �ڱ� �ڽ��� �ƴ� �÷��̾� �� ��쿡��, ���� �����͵��� �������� �ش��ϴ� �÷��̾��� ��ġ�� �����־�� ��.

		auto& myData = Network::GetInst()->GetNetworkObjMap().find(_networkId)->second;

		if (myData->GetIsInvincible())
		{
			this->GetTransform()->SetLocalPosition(Vec3(0.f, -4000.f, 0.f));
			break;
		}

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
