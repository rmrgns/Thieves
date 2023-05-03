#include "pch.h"
#include "NetworkSystem.h"
// Server
#include "server/main/network.h"
#include "server/ptr.h"
#include "server/main/packet_manager.h"

NetworkSystem::NetworkSystem() : Component(COMPONENT_TYPE::PARTICLE_SYSTEM)
{

}

NetworkSystem::~NetworkSystem()
{
}

void NetworkSystem::Update()
{
	
}
