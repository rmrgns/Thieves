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

	////서버로 부터 수신된 메시지 이벤트 처리
	// //현재 실행 중인 레벨 객체와 연결된 액터들에게 패킷 이벤트를 전달합니다.
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

	////서버로 메시지 이벤트를 전송
	// 현재 실행 중인 레벨 객체에서 디폴트 폰 객체를 가져와 해당 폰의 위치와 회전 값을 서버에 보내줍니다.
	//void PacketEventManager::SendEventToServer()
	//{
	//	while (m_server_receive_message_queue.empty() == false)
	//	{
	//		SPtr<MessageEventInfo> message;
	//		if (m_server_receive_message_queue.try_pop(message))
	//		{
	//			Network::GetInst()->SendMessageToServer(message);
	//			//SendManager::GetInst()->SendMessageToServer(message);
	//			//이벤트
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

	//			//여기서 플레이어 좌표랑 rotation 보내주기
	//			//LOG_INFO(pawn->GetPosition());
	//		}
	//	}
	//}
	//

	////ACTOR
	// 액터 객체에 패킷 이벤트를 등록하는 함수입니다. 액터의 ID 값과 함께 이벤트 메시지를 매개변수로 받습니다.
	//void PacketEventManager::RegisterPacketEventToActor(SPtr<MessageEventInfo>&& message, UINT id)
	//{
	//	m_server_send_message_to_actor_queue.push({ message, id });
	//}
	//// LEVEL
	// //레벨 객체에 패킷 이벤트를 등록하는 함수입니다. 이벤트 메시지를 매개변수로 받습니다.
	//void PacketEventManager::RegisterPacketEventToLevel(SPtr<MessageEventInfo>&& message)
	//{
	//	m_server_send_message_to_level_queue.push(message);
	//}
	////SERVER packet event
	////  서버에 패킷 이벤트를 등록하는 함수입니다. 이벤트 메시지를 매개변수로 받습니
	//void PacketEventManager::RegisterPacketEventToServer(SPtr<MessageEventInfo>&& message)
	//{
	//	m_server_receive_message_queue.push(message);
	//}

	//// Acotr를 서버에 연결하는 기능
	//// 액터 객체를 서버에 연결하는 함수입니다. 액터의 ID 값과 함께 액터 포인터를 매개변수로 받습니다.
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

	////스마트 포인터인 SPtr을 사용하여 메모리 관리를 처리하고, 
	////try_pop() 함수를 사용하여 큐에서 패킷을 빼오는 것을 확인
	////서버로부터 액터 객체를 연결 해제하는 함수입니다. 액터의 ID 값이 매개변수로 전달되며, 연결이 해제된 액터 포인터를 반환합니다.
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