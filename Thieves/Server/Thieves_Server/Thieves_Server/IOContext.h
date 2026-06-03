#pragma once
#include <WinSock2.h>
#include <coroutine>
#include <Windows.h>

// Awaiter에서 쓸 IOContext 클래스
// 기존의 OVERLAPPED 구조체 역할을 대신해준다.
class IOContext
{
private:
	WSAOVERLAPPED exp_over;
	std::coroutine_handle<> m_Handle;
	DWORD bytesTransferred = 0;
	bool isSend; // Send는 fire-and-forget이므로, Send 작업인지 여부를 저장



public:
	IOContext(bool is_send = false) : bytesTransferred(0), isSend(is_send) {
		ZeroMemory(&exp_over, sizeof(exp_over));
		m_Handle = nullptr;
	}

	std::coroutine_handle<>* GetHandle()  { return &m_Handle;}

	WSAOVERLAPPED* GetOverLapped() { return &exp_over; }

	DWORD GetBytesTransferred() const {	return bytesTransferred;}

	bool IsSend() const { return isSend; }

	void SetHandle(std::coroutine_handle<>& handle) { m_Handle = handle; }

	void setBytesTransferred(DWORD bytes) {
		bytesTransferred = bytes;
	}
};

