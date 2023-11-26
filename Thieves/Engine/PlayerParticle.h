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
	void PlayerGetitem();
	void PlayerShot();
	void PlayerTrapped();

private:
	float	_attackCount = 0.f;
	int32	_attackState = 0;		// 0=���ݾ���, 1=���ݹ�ư ����, 2=���� ��
	Vec3	_currentPos;
	int32	_trappedState = 0;
};

