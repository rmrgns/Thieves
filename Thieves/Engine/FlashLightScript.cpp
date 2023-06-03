#include "pch.h"
#include "FlashLightScript.h"
#include "Transform.h"
#include "Input.h"
#include "Engine.h"
#include "Timer.h"
#include "Light.h"
#include "GameObject.h"

// Temp
#include "SceneManager.h"



#include "server/main/network.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/main/network_move_object.h"
#include "server/thieves_server/game_info.h"

FlashLightScript::FlashLightScript()
{
}

FlashLightScript::~FlashLightScript()
{
}

void FlashLightScript::LateUpdate()
{
	Vec3 pos(0.0f, 0.0f, 0.0f);
	
	int myID = Network::GetInst()->GetPacketManager()->GetGameInfo().GetNetworkID();
	
	if (myID != -1) pos = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetPosition();
	//if (myID != -1) rotation = Network::GetInst()->GetNetworkObjMap().find(myID)->second->GetRotation();
	pos.y += 75.f;
	// Player의 LookVector를 가져온다
	Vec3 look = GET_SINGLE(SceneManager)->GetLookVec();

	// Rotation
	this->GetGameObject()->GetLight()->SetLightDirection(look);
	
	// Translation
	GetTransform()->SetLocalPosition(pos);
}