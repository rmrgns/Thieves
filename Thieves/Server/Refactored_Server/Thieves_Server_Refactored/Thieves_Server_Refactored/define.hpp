#pragma once
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <chrono>
#include <concurrent_priority_queue.h>
#include <random>

#include "protocol.hpp"
#include "state.hpp"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"

#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "ws2_32.lib")

const int BUFSIZE = 2048;

struct timer_event {
	int obj_id;
	int room_id;
	std::chrono::system_clock::time_point	start_time;
	EVENT_TYPE ev;
	int target_id;
	//구조체에 대한 연산자 오버로딩
		// start_time 이벤트 발생해야할 시간 저장
		// 이 함수는 이벤트의 start_time 멤버 변수를 기준 timer_event 객체를 비교하여 작은 것을 반환
		// timer_event 객체를 정렬 할때 사용하여 가장 가까운 시간에 발생해야 할 이벤트를 우선적으로 처리
	constexpr bool operator < (const timer_event& _Left) const
	{
		return (start_time > _Left.start_time);
	}
};

struct db_task {
	int obj_id;
	DB_TASK_TYPE dt;
	char user_id[MAX_NAME_SIZE];
	char user_password[MAX_PASSWORD_SIZE];
};