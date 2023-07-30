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

	void SetIsTimerStart(bool val) { m_isTimerStart = val; }
	bool GetIsTimerStart() { return m_isTimerStart; }

	void SetIsOpenSafe(bool val) { m_isOpenSafe = val; }
	bool GetIsOpenSafe() { return m_isOpenSafe; }

	void SetOpenSafeTime(std::chrono::system_clock::time_point val) { m_openSafeTime = val; }
	std::chrono::system_clock::time_point GetOpenSafeTime() { return m_openSafeTime; }

	void SetIsActiveEscape(bool val) { m_isActiveEscape = val; }
	bool GetIsActiveEscape() { return m_isActiveEscape; }

	void SetActiveEscapeTime(std::chrono::system_clock::time_point val) { m_ActiveEscapeTime = val; }
	std::chrono::system_clock::time_point GetActiveEscapeTime() { return m_ActiveEscapeTime; }

	void SetIsActiveSpecialEscape(bool val) { m_isActiveSpecialEscape = val; }
	bool GetIsActiveSpecialEscape() { return m_isActiveSpecialEscape; }

	void SetActiveSpecialEscapeTime(std::chrono::system_clock::time_point val) { m_ActiveSpecialEscapeTime = val; }
	std::chrono::system_clock::time_point GetActiveSpecialEscapeTime() { return m_ActiveSpecialEscapeTime; }

	void SetPhaseChanged(bool val) { m_isPhaseChanged = val; }
	bool GetPhaseChanged() { return m_isPhaseChanged; }

	void SetPhaseChangedTime(std::chrono::system_clock::time_point val) { m_PhaseChangedTime = val; }
	std::chrono::system_clock::time_point GetPhaseChangedTime() { return m_PhaseChangedTime; }

private:
	bool m_isAllPlayerLoaded = false;
	bool m_isGetAllObjInfo = false;
	bool m_isGetTime = false;
	bool m_isOpenSafe = false;
	bool m_isActiveEscape = false;
	bool m_isActiveSpecialEscape = false;
	bool m_isTimerStart = false;
	bool m_isPhaseChanged = false;
	std::chrono::system_clock::time_point m_startTime;
	std::chrono::system_clock::time_point m_openSafeTime;
	std::chrono::system_clock::time_point m_ActiveEscapeTime;
	std::chrono::system_clock::time_point m_ActiveSpecialEscapeTime;
	std::chrono::system_clock::time_point m_PhaseChangedTime;
	int m_item_num = -1;
};

