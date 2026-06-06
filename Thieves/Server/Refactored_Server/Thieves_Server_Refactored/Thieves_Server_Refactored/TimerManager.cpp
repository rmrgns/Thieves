#include "TimerManager.hpp"

void TimerManager::Push(int delay, std::function<void()> task)
{
	std::chrono::system_clock::time_point execTime = std::chrono::system_clock::now() + std::chrono::milliseconds(delay);

	{
		std::lock_guard<std::shared_mutex> lock(m_Lock);
		timerQueue.push({ execTime, std::move(task) });
	}

	m_CV.notify_one();
}

void TimerManager::WorkerThread()
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
