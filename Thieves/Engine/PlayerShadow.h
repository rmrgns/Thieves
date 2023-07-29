#pragma once
#include "MonoBehaviour.h"

class PlayerShadow : public MonoBehaviour
{
public:
	PlayerShadow();
	virtual ~PlayerShadow();

	virtual void LateUpdate() override;

private:

};