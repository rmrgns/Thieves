#pragma once
#include "Scene.h"
class InGameScene : public Scene
{
public:
	void SetAllPlayerLoaded(bool val) { m_isAllPlayerLoaded = val; };
	bool IsAllPlayerLoaded() { return m_isAllPlayerLoaded; }

private:
	bool m_isAllPlayerLoaded = false;
};

