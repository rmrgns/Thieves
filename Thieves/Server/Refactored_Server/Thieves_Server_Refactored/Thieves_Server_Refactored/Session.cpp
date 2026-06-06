#include "Session.hpp"
#include <ws2def.h>
#include <string.h>
#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include <span>
#include "CoroutineTypes.h"
#include "define.hpp"
#include "PacketManager.hpp"
#include "State.hpp"
#include "SendContextManager.h"

void Session::SetStateCallback(std::function<void(int)> callback)
{
	stateCallback = callback;
}

void Session::Init(int id, SOCKET socket)
{	
	m_SessionId = id;
	m_Socket = socket;

	m_State.store(static_cast<int>(S_STATE::ST_ALLOC));

	m_RecvCtx.InitHandle();
	ZeroMemory(m_RecvCtx.GetOverLapped(), sizeof(*m_RecvCtx.GetOverLapped()));
	ZeroMemory(m_RecvBuf, sizeof(m_RecvBuf));
}

Task Session::Run()
{
	int remainBytes = 0;
	std::cout << "[" << m_SessionId << "] Start, State is " << m_State.load() << "\n";

	while (m_State.load() == static_cast<int>(S_STATE::ST_ALLOC))
	{
		WSABUF wsaBuf { 
			.len = static_cast<ULONG>(BUFSIZE - remainBytes), 
			.buf = m_RecvBuf + remainBytes 
		};

		int numBytes = co_await AsyncRecv{ m_Socket, wsaBuf , &m_RecvCtx };

		std::cout << "[" << m_SessionId << "] awake. \n";

		if (numBytes <= 0) break;

		remainBytes += numBytes;

		std::span<char> dataView(m_RecvBuf, remainBytes);

		while (!dataView.empty())
		{
			int packetSize = static_cast<unsigned char>(dataView[0]); // 패킷의 첫 번째 바이트는 패킷의 크기를 나타냄

			// 이상한 패킷 오면 막아줘야 함
			if (packetSize == 0 || packetSize > BUFSIZE)
			{
				Disconnect();
				co_return;
			}

			if (packetSize <= remainBytes)
			{
				// 패킷 크기만큼만 자른 span 만들기.
				std::span<char> packetView = dataView.subspan(0, packetSize);

				// 딱 span만 보내기
				PacketManager::GetInst().ProcessPacket(m_SessionId, packetView);

				// 처리한 거 만큼 view를 아예 뒤로 밀어버리기
				// 그럼 += 같은거 할 이유 없음
				dataView = dataView.subspan(packetSize);

			}
			else {
				// 패킷이 잘렸다는 거니까, 다음 recv에서 받아야 함.
				break;
			}
		}

		remainBytes = static_cast<int>(dataView.size());

		if (remainBytes > 0)
		{
			memmove(m_RecvBuf, dataView.data(), remainBytes);
		}
	}

	Disconnect();
}

void Session::SendRaw(void* packet, int size)
{
	if (m_State.load() != static_cast<int>(S_STATE::ST_ALLOC)) return;

	SendContext* sendCtx = SendContextManager::GetInst().Pop(packet, size);

	DWORD sentBytes = 0;
	int result = WSASend(m_Socket, sendCtx->GetWsaBuf(), 1, 0, 0, sendCtx->GetOverLapped(), 0);

	if (result == SOCKET_ERROR)
	{
		int errorCode = WSAGetLastError();
		if (errorCode != WSA_IO_PENDING) {
			std::cout << "[" << GetId() << "] Something Wrong.\n";
			SendContextManager::GetInst().Push(sendCtx);
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

		// TODO:
		// 패킷매니저에서도 뭔가 해줘야 함.

		if (stateCallback) {
			stateCallback(m_SessionId);
		}
	}
}
