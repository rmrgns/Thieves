#pragma once
#include <WinSock2.h>
#include <mutex>
#include "CoroutineTypes.h"
#include "PacketManager.hpp"
#include <ws2def.h>
#include <atomic>
#include "define.hpp"
#include <Windows.h>
#include <string.h>
#include <functional>
#include "State.hpp"
#include <iostream>

class Session
{
private:
	int m_SessionId;
	SOCKET m_Socket;
	std::atomic<int> m_State;

	IOContext m_RecvCtx;
	char m_RecvBuf[BUFSIZE];

	std::function<void(int)> stateCallback;


public:

	Session() : m_SessionId(-1), m_Socket(INVALID_SOCKET), m_State(static_cast<int>(S_STATE::ST_FREE)) {};

	void Init(int id, SOCKET socket);

	int GetId() const { return m_SessionId; };
	Task Run();
	void Send(void* packet, int size);
	void Disconnect();

};

