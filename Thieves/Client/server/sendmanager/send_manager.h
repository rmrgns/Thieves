#pragma once
#include "../define.h"


#include "../massage/message._event_info.h"
class SendManager
{
public:
	SendManager() = default;
	virtual ~SendManager() = default;

	virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message) {};
	//virtual void SendMovePacket(const SOCKET& s_socket, const client_fw::Vec3& position, const client_fw::Quaternion& rotation) {};
	void SendPacket(const SOCKET& s_socket, int num_byte, void* packet);

};

