#include "pch.h"
#include "DiamondScript.h"
#include "server/main/network.h"
#include "Transform.h"
#include "InGameScene.h"
#include "SceneManager.h"


void DiamondScript::Update()
{
	auto iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());

	if (iScene->GetPhaseChanged())
	{
		int occupied_id = Network::GetInst()->GetPacketManager()->GetDiamondPlayer();

		if (occupied_id == -1) {
			this->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
			return;
		}

		Vec3 pos = Network::GetInst()->GetNetworkObjMap().find(occupied_id)->second->GetPosition();

		this->GetTransform()->SetLocalPosition(Vec3(pos.x, pos.y + 250.f, pos.z));
	}
	else {
		this->GetTransform()->SetLocalPosition(Vec3(0.f, 100.f, 0.f));
	}
}
