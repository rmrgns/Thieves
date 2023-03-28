#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"

#include "ptr.h"
#include "server/network/network.h"
#include "server/packet/packet_manager.h"
#include "server/send_manager/send_manager.h"
#include "thieves_server/thieves_packet/thieves_packet_manager.h"
#include "thieves_server/thieves_send/thieves_send_manager.h"

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
	GET_SINGLE(SceneManager)->LoadScene(L"LoginScene");

	Network::GetInst()->Init(client_fw::CreateUPtr<ThievesPacketManager>(), client_fw::CreateUPtr<ThievesSendManager>());
	Network::GetInst()->CreateWorker();

}

void Game::Update()
{
	GEngine->Update();
}
