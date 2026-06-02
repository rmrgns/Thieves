#pragma once
#include <WinSock2.h>
#include <span>
#include "IOContext.h"
#include <coroutine>

class AsyncRecv
{
	SOCKET socket;
	std::span<char> recvBuffer;
	IOContext* ioContext;

	// 코루틴 규약을 만족하기 위해 필요한 멤버 함수들
	// await_ready는 코루틴이 대기할 필요가 있는지를 결정하는 함수.
	bool await_ready() const noexcept;

	// await_suspend는 코루틴이 중단 되었을 때 호출되는 함수.
	void await_suspend(std::coroutine_handle<>& handle);

	// await_resume은 코루틴이 다시 실행될 때 호출되는 함수
	// 어떤 값을 돌려 줄건지 알려 주는 거.
	int await_resume() const noexcept { return ioContext->GetBytesTransferred(); }
};

