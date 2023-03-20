#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "SceneManager.h"
#include "../Client/server/network.h"

void Game::Init(const WindowInfo& info)
{
	GEngine->Init(info);
	//Network::GetInst()->Init(CreateUPtr<RevivePacketManager>(), CreateUPtr<ReviveSendManager>());
	Network::GetInst()->CreateWorker();

	GET_SINGLE(SceneManager)->LoadScene(L"LoginScene");
}

void Game::Update()
{
	GEngine->Update();
}
