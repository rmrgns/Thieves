#include "pch.h"
//
//#include "server/packet/packet_helper.h"
//#include "server/packet/packetevent/packet_event_manager.h"
//#include "server/event/event_system.h"
//
//namespace client_fw
//{
//	//�����κ��� ���� �޽����� ���Ϳ� ����ϴ� �Լ��Դϴ�. �޽����� ������ id�� �Ű������� �޾Ƽ�, 
//	//EventSystem�� ���� PacketEventManager�� �ش� �޽����� id�� ����մϴ�.
//	void PacketHelper::RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToActor(std::move(message), id);
//	}
//
//
//	// �����κ��� ���� �޽����� ������ ����ϴ� �Լ��Դϴ�. �޽����� �Ű������� �޾Ƽ�,
//	// EventSystem�� ���� PacketEventManager�� �ش� �޽����� ����մϴ�.
//	void PacketHelper::RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToLevel(std::move(message));
//	}
//
//	//Ŭ���̾�Ʈ���� ������ �޽����� ������ �Լ��Դϴ�.�޽����� �Ű������� �޾Ƽ�,
//	//EventSystem�� ���� PacketEventManager�� �ش� �޽����� ����ϰ�, Network�� ���� ������ �޽����� �����ϴ�.
//	void PacketHelper::RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->RegisterPacketEventToServer(std::move(message));
//	}
//	
//	//���͸� ������ �����ϴ� �Լ��Դϴ�. ���Ϳ� id�� �Ű������� �޾Ƽ�, EventSystem�� ����
//	//PacketEventManager�� ���Ϳ� id�� ����ϰ�, ���͸� ������ �����մϴ�.
//	/*void PacketHelper::ConnectActorToServer(const SPtr<Actor>& actor, UINT id)
//	{
//		EventSystem::GetEventSystem().GetPacketEventManager()->ConnectActorToServer(actor, id);
//	}*/
//
//	// �Լ��� ���͸� �������� ���� �����ϴ� �Լ��Դϴ�.id�� �Ű������� �޾Ƽ�,
//	//EventSystem�� ���� PacketEventManager���� �ش� id�� ���� ���͸� �������� ���� �����ϰ�,�ش� ���͸� ��ȯ�մϴ�.
//	//SPtr<Actor> PacketHelper::DisconnectActorFromServer(UINT id)
//	//{
//	//	return EventSystem::GetEventSystem().GetPacketEventManager()->DisconnectActorFromServer(id);
//	//}
//}