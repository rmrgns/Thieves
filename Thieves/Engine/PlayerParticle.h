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
	int32	_attackState = 0;		// 0=���ݾ���, 1=���ݹ�ư ����, 2=���� ��
};

