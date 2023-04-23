#pragma once
#include "../Client/ptr.h"
#include <minwindef.h>


	class MessageEventInfo;
	class Actor;

	/// PacketEvent�� client�� server���� ����� ���� ������ Message�� �ʿ��� �� ����� �� �ִ�.
	// ����ϱ� ���� ����� RegisterPacketEventTo[X]�� ���ؼ� message�� ����ϰ� �Ǹ�
	// Level�� Server�� ���� Message�� �ٷ� �Ѿ�� Actor�� ���� Message�� id(server�� ��ϵ�)�� ���ؼ� ���ٸ� Message�� �ް� �ȴ�.
	// 
	// Actor�� ���� ������ ������ �Ƿ��� ConnectActorToServer�Լ��� ȣ���ؼ� Actor�� ����԰� ���ÿ� server���� ������ id��
	// ���� ����ؾ��Ѵ�.
	// 
	// �� Server���� Actor�� Spawn�� �� ���� id�� �ް� �� id�� ���� connect�� �ϸ� �ȴ�.
	// 
	// �׷��� ������ message�� Actor�� Level�δ� ExecuteMessageFromServer�� ȣ���� ���ؼ� Message�� ������ �Ѿ��.
	// ���� �� Message�� ���� ó���� ExecuteMessageFromServer�Լ����� �ϸ� �ȴ�.
	//

	class PacketHelper
	{
	public:
		static void RegisterPacketEventToActor(shared_ptr<MessageEventInfo>&& message, UINT id);
		static void RegisterPacketEventToLevel(shared_ptr<MessageEventInfo>&& message);
		static void RegisterPacketEventToServer(shared_ptr<MessageEventInfo>&& message);

		static void ConnectActorToServer(const shared_ptr<Actor>& actor, UINT id);
		static shared_ptr<Actor> DisconnectActorFromServer(UINT id);
	};

