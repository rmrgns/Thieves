#pragma once
#include <WinSock2.h>
#include <coroutine>
#include <ws2def.h>
#include "define.hpp"
#include <string.h>
#include <Windows.h>
#include <exception>

// 코루틴 리턴 타입. 보일러플레이트 코드이므로 공식처럼 쓰면 됨.
struct Task {
	struct promise_type {
		Task get_return_object() { return {}; }
		// initial_suspend를 suspend_never로 하면, 호출되자마자 즉시 실행
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() { std::terminate(); }
	};
};

// Awaiter에서 쓸 IOContext 클래스
// 기존의 OVERLAPPED 구조체 역할을 대신해준다.
class IOContext
{
private:
	WSAOVERLAPPED m_Overlapped;
	std::coroutine_handle<> m_Handle;
	DWORD bytesTransferred = 0;
	bool isSend; // Send는 fire-and-forget이므로, Send 작업인지 여부를 저장



public:
	IOContext(bool isSend = false) : bytesTransferred(0), isSend(isSend) {
		ZeroMemory(&m_Overlapped, sizeof(m_Overlapped));
		m_Handle = nullptr;
	}

	std::coroutine_handle<>* GetHandle() { return &m_Handle; }

	WSAOVERLAPPED* GetOverLapped() { return &m_Overlapped; }

	DWORD GetBytesTransferred() const { return bytesTransferred; }

	bool IsSend() const { return isSend; }

	void SetSend(bool value) { isSend = value; }

	void SetHandle(std::coroutine_handle<>& handle) { m_Handle = handle; }

	void InitHandle() { m_Handle = nullptr; }

	void setBytesTransferred(DWORD bytes) {
		bytesTransferred = bytes;
	}
};

// Send 전용 버퍼를 가진 클래스 (기존의 EXP_OVER의 역할)
class SendContext : IOContext
{
	WSABUF wsaBuf;
	char sendBuf[BUFSIZE];

public:
	SendContext(void* packet, int size) : IOContext(true) {
		wsaBuf.len = size;
		wsaBuf.buf = sendBuf;

		if (size <= BUFSIZE) memcpy(sendBuf, packet, size);
	}

	WSABUF* GetWsaBuf() { return &wsaBuf; }

	char* GetSendBuffer() { return sendBuf; }

	WSAOVERLAPPED* GetOverLapped() { return IOContext::GetOverLapped(); }

};

class AsyncRecv
{
	SOCKET m_Socket;
	WSABUF m_RecvBuffer;
	IOContext* m_Ctx;
public:

	AsyncRecv(SOCKET, WSABUF, IOContext*) noexcept {};
	// 코루틴 규약을 만족하기 위해 필요한 멤버 함수들
	// await_ready는 코루틴이 대기할 필요가 있는지를 결정하는 함수.
	bool await_ready() const noexcept { return false; };

	// await_suspend는 코루틴이 중단 되었을 때 호출되는 함수.
	void await_suspend(std::coroutine_handle<> handle)
	{
		m_Ctx->SetHandle(handle);
		DWORD flags = 0;
		WSARecv(m_Socket, &m_RecvBuffer, 1, 0, &flags, m_Ctx->GetOverLapped(), 0);
	}

	// await_resume은 코루틴이 다시 실행될 때 호출되는 함수
	// 어떤 값을 돌려 줄건지 알려 주는 거.
	int await_resume() const noexcept { return m_Ctx->GetBytesTransferred(); }
};

// Accept 전용 문맥 구조체를 따로 정의
class AcceptContext : public IOContext {
	SOCKET clientSocket;
	char acceptBuf[1024];
public:
	AcceptContext() : IOContext(false), clientSocket(INVALID_SOCKET) {}

	SOCKET* GetSocket() { return &clientSocket; };
	char* GetBuffer() { return acceptBuf; }

	void SetSocket(SOCKET* sock) { clientSocket = *sock; }
	void MakeNewSocket() {
		clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	}
	
};


