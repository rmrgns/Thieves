#pragma once
#include "MonoBehaviour.h"

class RoomScript : public MonoBehaviour
{
public:
	virtual void Update() override;

public:
	void ClickRoomButton();
};

