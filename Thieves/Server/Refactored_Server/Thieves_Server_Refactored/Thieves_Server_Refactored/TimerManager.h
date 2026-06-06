#pragma once
#include <shared_mutex> 
#include <chrono>
#include <functional>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <utility>

struct TimerEvent {
	std::chrono::system_clock::time_point executeTime;
	std::function<void()> task;

	bool operator<(const TimerEvent& other) const {
		return executeTime > other.executeTime;
	}
};

class TimerManager
{
private:
	std::priority_queue<TimerEvent> timerQueue;
	// shared_mutex를 사용하면, 윈도우즈 환경에서는 자동적으로
	// SRWLOCK (Slim Reader/writer Lock) 이라는 락을 사용함.
	// 락 경합이 없으면 커널로 안들어가고, 그냥 CAS 한번으로 락 획득함.
	// ~ C++17
	std::shared_mutex m_Lock;

	// 특정 조건을 달성할 때 까지 쓰레드 실행을 대기하기 위한 클래스.
	// std::condition_variable m_CV; -> 기본 std::mutex를 쓸때만 사용 가능함.
	std::condition_variable_any m_CV; // -> lock()과 unlock() 함수만 구현되어 있으면 됨.

	// any 쓰면 느려지는거 아님?
	// -> 윈도우 컴파일러가 최적화 해줌 ㅇㅇ

	std::jthread workerThread;
	bool isRunning = true;

	TimerManager() {
		workerThread = std::jthread([this]() {WorkerThread(); });
	}

	~TimerManager() {
		isRunning = false;
		m_CV.notify_all();
	}

	TimerManager(const TimerManager&) = delete;
	TimerManager& operator=(const TimerManager&) = delete;

public:
	static TimerManager& GetInst() {
		static TimerManager s;
		return s;
	}

	void Push(int delay, std::function<void()> task) {
		std::chrono::system_clock::time_point execTime = std::chrono::system_clock::now() + std::chrono::milliseconds(delay);

		{
			std::lock_guard<std::shared_mutex> lock(m_Lock);
			timerQueue.push({ execTime, std::move(task) });
		}

		m_CV.notify_one();
	}

private:

	void WorkerThread()
	{
		while (isRunning) {
			std::unique_lock<std::shared_mutex> lock(m_Lock);

			if (timerQueue.empty())
			{
				m_CV.wait(lock);
				continue;
			}

			TimerEvent top = timerQueue.top();
			auto now = std::chrono::system_clock::now();

			if (top.executeTime <= now) {
				timerQueue.pop();
				lock.unlock();

				top.task();
			}
			else
			{
				m_CV.wait_until(lock, top.executeTime);
			}
		}
	}

};

