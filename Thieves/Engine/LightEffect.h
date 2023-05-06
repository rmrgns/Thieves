#pragma once

#include "MonoBehaviour.h"

class LightEffect : public MonoBehaviour
{
public:
	LightEffect();
	~LightEffect();

	virtual void LateUpdate() override;


private:
	bool	_lightState = true;
	bool	_lightStateDir = true;
	bool	_lightStateSpt = true;
	bool	_lightStatePnt = true;
};

