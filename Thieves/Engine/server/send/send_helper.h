#pragma once
#include "server/define.h"
#include "server/message/message_event_info.h"
class SendManager
{
public:
	SendManager() = default;
	virtual ~SendManager() = default;

	//virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<client_fw::MessageEventInfo>& message) {};


	virtual void TestProcessSend(const SOCKET& s_socket);
	

};

