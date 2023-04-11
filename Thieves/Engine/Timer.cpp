#include "pch.h"
#include "Timer.h"
#include <chrono>
#include <thread>

void Timer::Init()
{
	::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_frequency));
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_prevCount)); // CPU 클럭
}

void Timer::Update()
{
	uint64 currentCount;
	::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));

	_deltaTime = (currentCount - _prevCount) / static_cast<float>(_frequency);

	// 프레임 제한 코드
	if (_lockFPS > 0.0f)
	{
		while (_deltaTime < (1.0f / _lockFPS))
		{
			::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currentCount));
			_deltaTime = float((currentCount - _prevCount) / static_cast<float>(_frequency));
		}
	}

	_prevCount = currentCount;

	_frameCount++;
	_frameTime += _deltaTime;

	if (_frameTime > 1.f)
	{
		_fps = static_cast<uint32>(_frameCount / _frameTime);

		_frameTime = 0.f;
		_frameCount = 0;
	}
}