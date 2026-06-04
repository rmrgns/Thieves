#pragma once
#include "IOContext.h"
#include "define.h"
#include <WinSock2.h>
#include "state.h"

/// <summary>
/// 기존 player 클래스에서,
/// 네트워킹과 관련된 부분만 분리한 클래스입니다.
/// 
/// player와 session이 나뉘어, 
/// session에서는 네트워킹을, player 에서는 데이터 관리만 담당하게 하도록
/// 하는 구조로 변경합니다.
/// </summary>
class Session
{
	IOContext m_RecvContext;
	unsigned char m_RecvBuf[BUFSIZE];
	SOCKET  m_socket;

	Session() : m_socket(INVALID_SOCKET) {}

	~Session() = default;

	void DoRecv();
	void DoSend(int num_bytes, void* mess);
	SOCKET& GetSock() { return m_socket; }
	void Init(SOCKET&);
};

