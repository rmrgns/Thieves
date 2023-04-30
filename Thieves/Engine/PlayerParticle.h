#pragma once

#include "MonoBehaviour.h"

class PlayerParticle : public MonoBehaviour
{
public:
	PlayerParticle();
	~PlayerParticle();

	virtual void LateUpdate() override;

public:
	void PlayerAttack();

private:
	float	_attackCount = 0.f;
	int32	_attackState = 0;		// 0=공격안함, 1=공격버튼 눌림, 2=공격 중
};

