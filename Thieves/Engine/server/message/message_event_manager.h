#pragma once
//#include "server/event/event_manager.h"
//#include <queue>
//namespace client_fw
//{
//	// MessageEvent�� client������ �ٸ� Actor�� Level���� ������ Message�� �ʿ��� �� ����� �� �ִ�.
//	// ����ϱ� ���� ����� message_helper.h �Լ��� �����ϱ� �ٶ���.
//	//
//	class MessageEventManager final : public IEventManager
//	{
//	public:
//		MessageEventManager();
//		virtual ~MessageEventManager();
//
//		MessageEventManager(const MessageEventManager&) = delete;
//		MessageEventManager& operator=(const MessageEventManager&) = delete;
//
//		virtual void ExecuteEvent() override;
//
//	public:
//		void RegisterMessageEvent(SPtr<MessageEventInfo>&& message);
//
//		//void RegisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
//	//	void UnregisterMessageReceiver(UINT event_id, const SPtr<Actor>& actor);
//
//	private:
//		std::queue<SPtr<MessageEventInfo>> m_message_queue;
//		//std::multimap<UINT, SPtr<Actor>> m_message_receive_actors;
//	};
//}