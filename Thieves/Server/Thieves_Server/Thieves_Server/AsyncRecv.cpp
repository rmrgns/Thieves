#include "AsyncRecv.h"
#include <ws2def.h>
#include <Windows.h>
#include <WinSock2.h>
#include <coroutine>
#include <iostream>
#include <ostream>


// 코루틴 규약을 만족하기 위해 필요한 멤버 함수들
// await_ready는 코루틴이 대기할 필요가 있는지를 결정하는 함수.
bool AsyncRecv::await_ready() const noexcept {
	// IOCP에게 맡기는 작업이므로 항상 false를 반환하여
	// 코루틴이 기다리도록 함.
	return false;
}

// await_suspend는 코루틴이 중단 되었을 때 호출되는 함수.
void AsyncRecv::await_suspend(std::coroutine_handle<>& handle) {
	// IOContext에 코루틴 핸들을 저장하여, IO 작업이 완료되었을 때
	// 해당 코루틴을 다시 실행할 수 있도록 함.
	ioContext->SetHandle(handle);
	ioContext->setBytesTransferred(0);

	WSABUF wsaBuf;
	wsaBuf.len = recvBuffer.size();
	wsaBuf.buf = recvBuffer.data();
	DWORD flags = 0;

	if (WSARecv(socket, &wsaBuf, 1, nullptr, &flags, ioContext->GetOverLapped(), nullptr) == SOCKET_ERROR) {
		int err = WSAGetLastError();
		if (err != WSA_IO_PENDING) {
			//TODO : 에러 처리
			WCHAR* lpMsgBuf = nullptr;
			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, err,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)&lpMsgBuf, 0, 0);
			std::wcout << lpMsgBuf << std::endl;
			LocalFree(lpMsgBuf);
		}
	}
}