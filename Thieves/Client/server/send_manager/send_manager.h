#pragma once
#include "server/define.h"
#include "server/message/message_event_info.h"
class SendManager
{
public:
	SendManager() = default;
	virtual ~SendManager() = default;

	//virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message) {};
	virtual void SendMovePacket(const SOCKET& s_socket, char direction,
		float f_x, float f_y, float f_z,
		float r_x, float r_y, float r_z,
		float deltatime) {};
	void SendPacket(const SOCKET& s_socket, int num_byte, void* packet);

};

