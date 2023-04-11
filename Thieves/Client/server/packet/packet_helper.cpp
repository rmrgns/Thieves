#include "pch.h"
//
//#include "server/packet/packet_helper.h"
//#include "server/packet/packetevent/packet_event_manager.h"
//#include "server/event/event_system.h"
//
//namespace client_fw
//{
//	//서버로부터 받은 메시지를 액터에 등록하는 함수입니다. 메시지와 액터의 id를 매개변수로 받아서, 
//	//EventSystem을 통해 PacketEventManager에 해당 메시지와 id를 등록합니다.
//	void PacketHelper::RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToActor(std::move(message), id);
//	}
//
//
//	// 서버로부터 받은 메시지를 레벨에 등록하는 함수입니다. 메시지를 매개변수로 받아서,
//	// EventSystem을 통해 PacketEventManager에 해당 메시지를 등록합니다.
//	void PacketHelper::RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToLevel(std::move(message));
//	}
//
//	//클라이언트에서 서버로 메시지를 보내는 함수입니다.메시지를 매개변수로 받아서,
//	//EventSystem을 통해 PacketEventManager에 해당 메시지를 등록하고, Network를 통해 서버로 메시지를 보냅니다.
//	void PacketHelper::RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToServer(std::move(message));
//	}
//	
//	//액터를 서버에 연결하는 함수입니다. 액터와 id를 매개변수로 받아서, EventSystem을 통해
//	//PacketEventManager에 액터와 id를 등록하고, 액터를 서버에 연결합니다.
//	/*void PacketHelper::ConnectActorToServer(const SPtr<Actor>& actor, UINT id)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->ConnectActorToServer(actor, id);
//	}*/
//
//	// 함수는 액터를 서버에서 연결 해제하는 함수입니다.id를 매개변수로 받아서,
//	//EventSystem을 통해 PacketEventManager에서 해당 id를 갖는 액터를 서버에서 연결 해제하고,해당 액터를 반환합니다.
//	//SPtr<Actor> PacketHelper::DisconnectActorFromServer(UINT id)
//	//{
//	//	return EventSystem::GetEventSystem().GetPacketEventManager()->DisconnectActorFromServer(id);
//	//}
//}