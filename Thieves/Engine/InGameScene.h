#pragma once
#include "Scene.h"
class InGameScene : public Scene
{
public:
	void SetAllPlayerLoaded(bool val) { m_isAllPlayerLoaded = val; };
	bool IsAllPlayerLoaded() { return m_isAllPlayerLoaded; }

	void SetAllObjInfo(bool val) { m_isGetAllObjInfo = val; };
	bool IsGetAllObjInfo() { return m_isGetAllObjInfo; };

	void SetIsGetTime(bool val) { m_isGetTime = val; };
	bool IsGetTime() { return m_isGetTime; };

	void SetStartTime(std::chrono::system_clock::time_point val) { m_startTime = val; }
	std::chrono::system_clock::time_point GetStartTime() { return m_startTime; }

	void SetItemNum(int val) { m_item_num = val; }
	int GetItemNum() { return m_item_num; }

private:
	bool m_isAllPlayerLoaded = false;
	bool m_isGetAllObjInfo = false;
	bool m_isGetTime = false;
	std::chrono::system_clock::time_point m_startTime;
	int m_item_num = -1;
};

