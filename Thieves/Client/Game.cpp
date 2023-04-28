#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"

#include "server/ptr.h"
#include "server/main/network.h"
#include "server/main/packet_manager.h"
#include "server/main/send_manager.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/thieves_server/thieves_send_manager.h"

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
