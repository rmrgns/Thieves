#pragma once
#include "define.h"
#include "IOContext.h"
#include <ws2def.h>
#include <string.h>
#include <WinSock2.h>


class SendContext : IOContext
{
	WSABUF wsaBuf;
	char sendBuf[BUFSIZE];

public:
	SendContext(char* packet) : IOContext(true) {
		wsaBuf.len = packet[0];
		wsaBuf.buf = sendBuf;
		memcpy(sendBuf, packet, packet[0]);
	}

	WSABUF* GetWsaBuf() { return &wsaBuf; }

	char* GetSendBuffer() { return sendBuf; }

	WSAOVERLAPPED* GetOverLapped() { return IOContext::GetOverLapped(); }

};

