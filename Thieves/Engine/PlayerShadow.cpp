#include "pch.h"
#include "PlayerShadow.h"
#include "Transform.h"

#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"


PlayerShadow::PlayerShadow()
{
}

PlayerShadow::~PlayerShadow()
{
}

void PlayerShadow::LateUpdate()
{
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	Vec3 pos(0.0f, 0.0f, 0.0f);
	if (myID != -1) pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
	
	Vec3 calculatedPos(0.0f, 0.0f, 0.0f);
	calculatedPos.x = pos.x * 0.93f;
	calculatedPos.z = pos.z * 0.93f;
	calculatedPos.y = 1000.f;

	GetTransform()->SetLocalPosition(calculatedPos);
}
