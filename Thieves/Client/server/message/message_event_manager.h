#pragma once
#include "server/event/event_manager.h"
#include <queue>

// MessageEvent는 client내에서 다른 Actor나 Level에게 보내는 Message가 필요할 때 사용할 수 있다.
// 사용하기 위한 방법은 message_helper.h 함수를 참조하길 바란다.
//
class MessageEventManager final : public IEventManager
{
public:
	MessageEventManager();
	virtual ~MessageEventManager();

	MessageEventManager(const MessageEventManager&) = delete;
	MessageEventManager& operator=(const MessageEventManager&) = delete;

	virtual void ExecuteEvent() override;

public:
	void RegisterMessageEvent(shared_ptr<MessageEventInfo>&& message);

	//void RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
//	void UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);

private:
	std::queue<shared_ptr<MessageEventInfo>> m_message_queue;
	std::multimap<UINT, shared_ptr<Actor>> m_message_receive_actors;
};
