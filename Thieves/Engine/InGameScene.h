#pragma once
#include "Scene.h"
class InGameScene : public Scene
{
public:
	void SetAllPlayerLoaded(bool val) { m_isAllPlayerLoaded = val; };
	bool IsAllPlayerLoaded() { return m_isAllPlayerLoaded; }

	void SetAllObjInfo(bool val) { m_isGetAllObjInfo = val; };
	bool IsGetAllObjInfo() { return m_isGetAllObjInfo; };

private:
	bool m_isAllPlayerLoaded = false;
	bool m_isGetAllObjInfo = false;
};

