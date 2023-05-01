#pragma once
#include "server/define.h"
#include "server/message/message_event_info.h"
class SendManager
{
public:
	SendManager() = default;
	virtual ~SendManager() = default;

	//virtual void ProcessSend(const SOCKET& s_socket, shared_ptr<MessageEventInfo>& message) {};
	
	// Send
	virtual void SendMovePacket(const SOCKET& s_socket, char direction,
		Vec3 pos, Vec3 vec, float deltatime, bool jumpstate) {};
	virtual void SendStartPacket(const SOCKET& s_socket) {};


	//virtual void TestProcessSend(const SOCKET& s_socket);
	void SendPacket(const SOCKET& s_socket, int num_byte, void* packet);

};

