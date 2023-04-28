#include "pch.h"
#include "message_event_info.h"
#include "server/main/network_move_object.h"

MoveObjectMessageEventInfo::MoveObjectMessageEventInfo(UINT event_id, const Vec3& pos)
	: MessageEventInfo(event_id), m_pos(pos)
{
}