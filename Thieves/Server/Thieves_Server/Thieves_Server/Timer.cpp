#include "Timer.h"
#include "EVENT.h"

void Timer::AddTimer(int objId, std::chrono::system_clock::time_point execTime, EVENT_TYPE eventType)
{
	EVENT NewEv{ objId, execTime, eventType };
	timerLock.lock();
	timerQ.push(NewEv);
	timerLock.unlock();
}
