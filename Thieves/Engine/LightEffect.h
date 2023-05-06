#pragma once

#include "MonoBehaviour.h"

class LightEffect : public MonoBehaviour
{
public:
	LightEffect();
	~LightEffect();

	virtual void LateUpdate() override;


private:
	bool	_lightState = false;
};

