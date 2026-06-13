#pragma once
#include <WinSock2.h>
#include <coroutine>
#include <ws2def.h>
#include "define.h"
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
// SList 활용해서 미리 많이 만들어 놓고 순서대로 꺼내 놓는 방식으로 진행할 거기 때문에,
// SList를 활용하기 위해 16바이트 단위로 정렬해두기.

using BroadcastBuffer = std::shared_ptr<std::vector<char>>; // 타이핑 길어지니까 이름 바꿔두기

class alignas(16) SendContext : public _SLIST_ENTRY, public IOContext
{
	WSABUF wsaBuf;
	std::vector<char> sendBuf;
	
	BroadcastBuffer broadcastBuf;

public:
	SendContext() : IOContext(true) {
		// 1024는 일단 정한 수치이므로, 나중에 변경해 주어야 함.
		sendBuf.reserve(1024);
	}

	void Setup(void* packet, int size)
	{
		char* p = static_cast<char*>(packet);

		sendBuf.assign(p, p + size);

		wsaBuf = {
			.len = static_cast<ULONG>(size),
			.buf = sendBuf.data()
		};

		ZeroMemory(GetOverLapped(), sizeof(*GetOverLapped()));
	}

	// 접속한 사람들에게 브로드캐스팅 해야 할때.
	// TCP의 특성상 다 한번씩 돌면서 send를 해줘야 하는데,
	// send 할때마다 데이터 만들어 버리면 그거도 그거대로 오버헤드임.
	// 그래서 shared_ptr 써서 데이터 하나로 유지하고, 아무도 안쓰면 스마트 포인터가 자동으로 삭제되게 하기
	void SetupBroadcast(BroadcastBuffer bBuf)
	{
		broadcastBuf = bBuf;

		wsaBuf = {
			.len = static_cast<ULONG>(broadcastBuf->size()),
			.buf = broadcastBuf->data()
		};

		ZeroMemory(GetOverLapped(), sizeof(*GetOverLapped()));
	}

	void Clear()
	{
		// 레퍼런스 카운트 감소 시켜줌.
		broadcastBuf.reset();
	}

	WSABUF* GetWsaBuf() { return &wsaBuf; }

	std::vector<char>* GetSendBuffer() { return &sendBuf; }

	WSAOVERLAPPED* GetOverLapped() { return IOContext::GetOverLapped(); }

};

class AsyncRecv
{
	SOCKET m_Socket;
	WSABUF m_RecvBuffer;
	IOContext* m_Ctx;
public:

	AsyncRecv(SOCKET sock, WSABUF wsaBuf, IOContext* ioCtx) noexcept : m_Socket(sock), m_RecvBuffer(wsaBuf), m_Ctx(ioCtx) {};
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
	AcceptContext() : IOContext(false), clientSocket(INVALID_SOCKET) {
	}

	SOCKET* GetSocket() { return &clientSocket; };
	char* GetBuffer() { return acceptBuf; }

	void SetSocket(SOCKET* sock) { clientSocket = *sock; }
	void MakeNewSocket() {
		clientSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	}
	
};


