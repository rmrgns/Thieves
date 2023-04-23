#pragma once
#include"server/protocol.h"
#include "message_event.h"

#include<mutex>

class MessageEventInfo
{
public:
	MessageEventInfo(UINT event_id)
		: m_event_id(event_id) {}

protected:
	UINT m_event_id;

public:
	UINT GetEventID() const { return m_event_id; }
};
}