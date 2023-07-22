#pragma once
#include <queue>
#include <mutex>
#include <chrono>
#include "state.h"
class EVENT;

class Timer
{
public:
	std::priority_queue<EVENT> timerQ;
	std::mutex timerLock;

public:

	void AddTimer(int objId, std::chrono::system_clock::time_point execTime, EVENT_TYPE eventType);


};

