#pragma once
#include "server/protocol.h"
#include "message_event.h"

#include<mutex>

class NetworkMoveObj;

class MoveObjectMessageEventInfo final :public MessageEventInfo
{
public:
    MoveObjectMessageEventInfo(UINT event_id, const Vec3& pos);

private:
    Vec3 m_pos;


public:
    const Vec3& GetObjPosition()const { return m_pos; }

};