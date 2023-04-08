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
	
	void SetAcc(bool flag) { _useAcc = flag; }
	bool IsAcc() { return _useAcc; }
	void Accelerate();
	void AddForce();

private:
	bool _useGravity = true;
	float _gravity = 9.8f;

	bool _useAcc = false;
	Vec3 _force = { 0,0,0 };
	Vec3 _vel;
	float _movingForce = 100.f;
};

