#pragma once
#include <chrono>
#include "state.h"

class EVENT
{
	int objId;
	int room_id;
	std::chrono::system_clock::time_point execTime;
	EVENT_TYPE eventType;
	int target_id;
public:

	EVENT(int id, std::chrono::system_clock::time_point time, EVENT_TYPE type) : objId(id), execTime(time), eventType(type)
	{};
	EVENT(int obj_id, int target_i, std::chrono::system_clock::time_point time, EVENT_TYPE type) : objId(id), execTime(time), eventType(type)
	{};

	std::chrono::system_clock::time_point GetExecTime() { return execTime; };
	
	int GetObjId() { return objId; };

	EVENT_TYPE GetEventType() { return eventType; };

	constexpr bool operator < (const EVENT& _Left) const
	{
		return (execTime > _Left.execTime);
	}

};

