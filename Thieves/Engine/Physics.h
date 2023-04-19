#pragma once
#include "Component.h"

class Physics : public Component
{
public:
	Physics();
	virtual ~Physics();

	virtual void FinalUpdate() override;
	
public:
	void SetGravity(bool flag) { _useGravity = flag; }
	bool IsGravity() { return _useGravity; }
	void UseGravity();

private:
	bool _useGravity = true;
	
	float _gravity = 9.8f;

	bool _useAcc = false;

};

