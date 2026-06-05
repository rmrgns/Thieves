#include "Session.hpp"
#include <ws2def.h>
#include <string.h>
#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include "CoroutineTypes.h"
#include "define.hpp"
#include "PacketManager.hpp"
#include "State.hpp"

void Session::SetStateCallback(std::function<void(int)> callback)
{
	stateCallback = callback;
}

void Session::Init(int id, SOCKET socket)
{
	m_State.store(static_cast<int>(S_STATE::ST_ALLOC));

	m_SessionId = id;
	m_Socket = socket;

	ZeroMemory(m_RecvCtx.GetOverLapped(), sizeof(*m_RecvCtx.GetOverLapped()));
	m_RecvCtx.InitHandle();
	ZeroMemory(m_RecvBuf, sizeof(m_RecvBuf));
}

Task Session::Run()
{
	int remainBytes = 0;
	std::cout << "Session Start " << m_SessionId << ", State is " << m_State.load() << "\n";

	while (m_State.load())
	{
		WSABUF wsaBuf;
		wsaBuf.len = BUFSIZE - remainBytes;
		wsaBuf.buf = m_RecvBuf + remainBytes;
		
		int numBytes = co_await AsyncRecv{ m_Socket, wsaBuf , &m_RecvCtx };

		std::cout << "[" << m_SessionId << "] awake. \n";

		if (numBytes <= 0) break;

		remainBytes += numBytes;
		char* p = m_RecvBuf;

		while (remainBytes > 0)
		{
			int packetSize = p[0]; // 패킷의 첫 번째 바이트는 패킷의 크기를 나타냄

			// 이상한 패킷 오면 막아줘야 함
			if (packetSize == 0 || packetSize > BUFSIZE)
			{
				Disconnect();
				co_return;
			}

			if (packetSize <= remainBytes)
			{
				PacketManager::GetInst().ProcessPacket(m_SessionId, (unsigned char*)p);

				p += packetSize;
				remainBytes -= packetSize;

			}
			else {
				// 패킷이 잘렸다는 거니까, 다음 recv에서 받아야 함.
				break;
			}
		}

		if (remainBytes > 0)
		{
			memmove(m_RecvBuf, p, remainBytes);
		}
	}

	Disconnect();
}

void Session::Send(void* packet, int size)
{
	if (m_State.load() != static_cast<int>(S_STATE::ST_ALLOC)) return;

	SendContext* sendCtx = new SendContext(packet, size);

	if (WSASend(m_Socket, sendCtx->GetWsaBuf(), 1, 0, 0, sendCtx->GetOverLapped(), 0) == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING) {
			std::cout << "Session " << GetId() << " Something Wrong.\n";
			delete sendCtx;
			Disconnect();
		}
	}
}

void Session::Disconnect()
{

	std::cout << "[" << m_SessionId << "] Disconnect. \n";
	int expected = static_cast<int>(S_STATE::ST_ALLOC);

	// 멀티 스레드 환경에서 문제가 있을 수 있으므로, CAS 연산으로 확실하게 확인.
	if (m_State.compare_exchange_strong(expected, static_cast<int>(S_STATE::ST_FREE)))
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;

		// TODO:
		// 패킷매니저에서도 뭔가 해줘야 함.

		if (stateCallback) {
			stateCallback(m_SessionId);
		}
	}
}
