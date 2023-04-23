#include "pch.h"
#include "server/message/message_helper.h"
#include "server/message/message_event_manager.h"

void MessageHelper::RegisterMessageEvent(shared_ptr<MessageEventInfo>&& message)
{
	EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageEvent(std::move(message));
}

void MessageHelper::RegisterMessageReceiver(UINT event_id, const shared_ptr<Actor>& actor)
{
	EventSystem::GetEventSystem().GetMessageEventManager()->RegisterMessageReceiver(event_id, actor);
}

void MessageHelper::UnregisterMessageReceiver(UINT event_id, const shared_ptr<Actor>& actor)
{
	EventSystem::GetEventSystem().GetMessageEventManager()->UnregisterMessageReceiver(event_id, actor);
}
