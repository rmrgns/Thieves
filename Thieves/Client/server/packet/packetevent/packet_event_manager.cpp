#include "pch.h"

#include "server/message/message_event_info.h"
#include "server/packet/packetevent/packet_event_manager.h"

#include "server/packet/packet_helper.h"
#include "server/network/network.h"

namespace client_fw
{
	//PacketEventManager::PacketEventManager()
	//{
	//}

	//PacketEventManager::~PacketEventManager()
	//{
	//}

	////������ ���� ���ŵ� �޽��� �̺�Ʈ ó��
	// //���� ���� ���� ���� ��ü�� ����� ���͵鿡�� ��Ŷ �̺�Ʈ�� �����մϴ�.
	//void PacketEventManager::ExecuteEventReceivedFromServer()
	//{
	//	const auto& app = Application::GetApplication();
	//	const auto& current_level = LevelManager::GetLevelManager().GetCurrentLevel();
	//	if (current_level != nullptr)
	//	{
	//		SPtr<MessageEventInfo> message;
	//		while (m_server_send_message_to_level_queue.empty() == false)
	//		{
	//			if (m_server_send_message_to_level_queue.try_pop(message))
	//				current_level->ExecuteMessageFromServer(message);
	//		}

	//		PacketIDEventInfo actor_message;
	//		while (m_server_send_message_to_actor_queue.empty() == false)
	//		{
	//			if (m_server_send_message_to_actor_queue.try_pop(actor_message) &&
	//				m_connected_actor_map[actor_message.id] != nullptr)
	//			{
	//				m_connected_actor_map[actor_message.id]->ExecuteMessageFromServer(actor_message.message);
	//			}
	//		}
	//	}
	//}

	////������ �޽��� �̺�Ʈ�� ����
	// ���� ���� ���� ���� ��ü���� ����Ʈ �� ��ü�� ������ �ش� ���� ��ġ�� ȸ�� ���� ������ �����ݴϴ�.
	//void PacketEventManager::SendEventToServer()
	//{
	//	while (m_server_receive_message_queue.empty() == false)
	//	{
	//		SPtr<MessageEventInfo> message;
	//		if (m_server_receive_message_queue.try_pop(message))
	//		{
	//			Network::GetInst()->SendMessageToServer(message);
	//			//SendManager::GetInst()->SendMessageToServer(message);
	//			//�̺�Ʈ
	//		}
	//	}

	//	const auto& current_level = LevelManager::GetLevelManager().GetCurrentLevel();
	//	if (current_level != nullptr)
	//	{
	//		SPtr<MessageEventInfo>move_message;
	//		const auto& pawn = current_level->GetGameMode()->GetDefaultPawn();
	//		if (pawn != nullptr && pawn->IsUpdatedWorldMatrix() && pawn->IsConnectedToServer())
	//		{
	//			if (true == Network::matching_end)
	//			{
	//				Network::GetInst()->SendMovePacket(pawn->GetPosition(), pawn->GetRotation());
	//			}

	//			//���⼭ �÷��̾� ��ǥ�� rotation �����ֱ�
	//			//LOG_INFO(pawn->GetPosition());
	//		}
	//	}
	//}
	//

	////ACTOR
	// ���� ��ü�� ��Ŷ �̺�Ʈ�� ����ϴ� �Լ��Դϴ�. ������ ID ���� �Բ� �̺�Ʈ �޽����� �Ű������� �޽��ϴ�.
	//void PacketEventManager::RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id)
	//{
	//	m_server_send_message_to_actor_queue.push({ message, id });
	//}
	//// LEVEL
	// //���� ��ü�� ��Ŷ �̺�Ʈ�� ����ϴ� �Լ��Դϴ�. �̺�Ʈ �޽����� �Ű������� �޽��ϴ�.
	//void PacketEventManager::RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message)
	//{
	//	m_server_send_message_to_level_queue.push(message);
	//}
	////SERVER packet event
	////  ������ ��Ŷ �̺�Ʈ�� ����ϴ� �Լ��Դϴ�. �̺�Ʈ �޽����� �Ű������� �޽���
	//void PacketEventManager::RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message)
	//{
	//	m_server_receive_message_queue.push(message);
	//}

	//// Acotr�� ������ �����ϴ� ���
	//// ���� ��ü�� ������ �����ϴ� �Լ��Դϴ�. ������ ID ���� �Բ� ���� �����͸� �Ű������� �޽��ϴ�.
	//void PacketEventManager::ConnectActorToServer(const SPtr<Actor>& actor, UINT id)
	//{
	//	if (m_connected_actor_map[id] != nullptr || actor->IsConnectedToServer())
	//		//LOG_WARN("This id is already registered with the server");
	//	else
	//	{
	//		m_connected_actor_map[id] = actor;
	//		actor->ConnectServer(true);
	//	}
	//}

	////����Ʈ �������� SPtr�� ����Ͽ� �޸� ������ ó���ϰ�, 
	////try_pop() �Լ��� ����Ͽ� ť���� ��Ŷ�� ������ ���� Ȯ��
	////�����κ��� ���� ��ü�� ���� �����ϴ� �Լ��Դϴ�. ������ ID ���� �Ű������� ���޵Ǹ�, ������ ������ ���� �����͸� ��ȯ�մϴ�.
	//SPtr<Actor> PacketEventManager::DisconnectActorFromServer(UINT id)
	//{
	//	if (m_connected_actor_map.find(id) != m_connected_actor_map.cend()
	//		&& m_connected_actor_map[id] != nullptr)
	//	{
	//		SPtr<Actor> actor = m_connected_actor_map[id];
	//		actor->ConnectServer(false);
	//		m_connected_actor_map[id] = nullptr;
	//		return actor;
	//	}
	//	else
	//	{
	//		//LOG_WARN("ID : {0} is already disconnected", id);
	//		return nullptr;
	//	}

	//}

}