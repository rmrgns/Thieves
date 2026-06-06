#pragma once
#include <windows.h>
#include "CoroutineTypes.h"


/// <summary>
/// 원래 하던대로 new, delete로 하면, 계속해서 메모리 받고 지우고 받고 지우고 하다보니까,
/// 메모리 단편화로 인해 느려지게 됨.
/// 
/// 그러므로, 미리 SList를 이용해서 많이 만들어 두고,
/// 필요할 때 꺼내 쓰고 반납하는 방식으로 해결하기.
/// </summary>
class SendContextManager
{
private:
	alignas(16) SLIST_HEADER sListHeader;

	SendContextManager() {
		InitializeSListHead(&sListHeader);

		// 10000개도 임의의 갯수 이므로, 나중에 변경해야함.
		for (int i = 0; i < 10000; ++i)
		{
			SendContext* ctx = new SendContext();
			InterlockedPushEntrySList(&sListHeader, ctx);
		}
	}

	~SendContextManager() = default;
	// 이동, 복사 막기 (싱글톤 할 때 생겨버릴 수 있으니까)
	SendContextManager(const SendContextManager&) = delete;
	SendContextManager& operator=(const SendContextManager&) = delete;

public:
	static SendContextManager& GetInst() {
		static SendContextManager s;
		return s;
	}

	SendContext* Pop(void* data, int size) {
		PSLIST_ENTRY entry = InterlockedPopEntrySList(&sListHeader);
		SendContext* ctx = nullptr;

		if (entry)
		{
			ctx = static_cast<SendContext*>(entry);
		}
		else
		{
			// 10000 개 넘었을 때 혹시 모를 일을 대비해서.
			ctx = new SendContext();
		}

		ctx->Setup(data, size);
		return ctx;
	}

	void Push(SendContext* ctx)
	{
		InterlockedPushEntrySList(&sListHeader, ctx);
	}
};

