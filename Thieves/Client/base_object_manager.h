#pragma once

class IBaseObjectManager
{
public:
	virtual void Shutdown() = 0;

	virtual void Update(float delta_time) = 0;
};