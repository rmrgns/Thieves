#pragma once

#include <string_view>

class IBaseObject
{
public:
	virtual bool Initialize() = 0;
	virtual void Shutdown() = 0;

	virtual void Update(float delta_time) = 0;
};
}