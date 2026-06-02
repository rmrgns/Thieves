#pragma once
#include <coroutine>

// 코루틴의 리턴 타입 ( 보일러플레이트 코드이므로 그냥 공식처럼 사용 )
struct Task {
	struct promise_type{
		Task get_return_object() { return {}; }
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void return_void() {}
		void unhandled_exception() {}
	};
};