#pragma once
#include <array>
#include "Session.h"
#include <Windows.h>
#include <optional>
#include "protocol.h"


__declspec(align(16)) struct SessionNode {
	SLIST_ENTRY ItemEntry;
	int SessionId;
};

class SessionManager
{
private:

	std::array<Session, MAX_USER> sessions;

	// Interlocked Singly Linked Lists -> SList를 활용함
	// 사실상 락프리 스택 같은 느낌
	// 16바이트로 정렬한 뒤, 카운터를 활용해서 ABBA 문제도 해결해 준다고 함.
	__declspec(align(16)) SLIST_HEADER freeSessionList;
	__declspec(align(16)) SessionNode sessionNodes[MAX_USER];

	SessionManager() {}
	~SessionManager() {}

public:

	static SessionManager& GetInst() {
		static SessionManager s;
		return s;
	}

	Session* GetSession(int sessionId)
	{
		if (sessionId < 0 || sessionId >= MAX_USER) return nullptr;
		return &sessions[sessionId];
	}

	void InitializeSessions();

	std::optional<int> GetEmptySessionId();

	void ReturnSessionId(int id);
};

