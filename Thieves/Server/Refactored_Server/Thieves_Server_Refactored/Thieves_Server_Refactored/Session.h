#pragma once
#include <WinSock2.h>
#include "CoroutineTypes.h"
#include <atomic>
#include "define.h"
#include <functional>
#include "State.h"

class Session
{
private:
	int m_SessionId;
	SOCKET m_Socket;
	std::atomic<int> m_State;

	IOContext m_RecvCtx;
	char m_RecvBuf[BUFSIZE];

	// 기존의 공간에서 데이터만 정리하는 형식으로 하고 있기 때문에, 빈 세션으로 바꾸기 위한 콜백 함수를 하나 만들기.
	std::function<void(int)> stateCallback;


public:

	template <typename T>
	void SendPacket(unsigned char packetId, T& packetData) {
		packetData.size = static_cast<unsigned char>(sizeof(T));
		packetData.type = packetId;

		this->SendRaw(&packetData, sizeof(T));

		DEBUG_LOG("[" << (int)packetId << " Packet ] Send \n");
	}

	template <typename T>
	void SendBroadcast(BroadcastBuffer bBuf) {
		
	}

	Session() : m_SessionId(-1), m_Socket(INVALID_SOCKET), m_State(static_cast<int>(S_STATE::ST_FREE)) {};

	void SetStateCallback(std::function<void(int)>);

	void Init(int id, SOCKET socket);

	int GetId() const { return m_SessionId; };
	int GetState() const { return m_State; };

	void SetState(int value) { m_State.store(value); };



	Task Run();
	void Disconnect();

private:
	void SendRaw(void* packet, int size);

};

