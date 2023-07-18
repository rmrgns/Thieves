#pragma once
#include "MonoBehaviour.h"

class UsingGun : public MonoBehaviour
{
public:
	UsingGun();
	virtual ~UsingGun();

	virtual void LateUpdate() override;

private:
};

