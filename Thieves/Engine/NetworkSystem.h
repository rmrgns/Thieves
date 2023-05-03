#pragma once
#include "Component.h"

enum class NetworkType
{
	NONE,
	PLAYER,
	OTHER_PLAYER,
	NPC,

};

class NetworkSystem : public Component
{
public:
	NetworkSystem();
	virtual ~NetworkSystem();

	virtual void Update();

	void SetNetworkingType(NetworkType type) { _nType = type; };
	NetworkType GetNetworkingType() { return _nType; };

	void SetNetworkId(int id) { _networkId = id; }
	int GetNetworkId() { return _networkId; }

private:
	int _networkId = -1;
	NetworkType _nType = NetworkType::NONE;
	

};

