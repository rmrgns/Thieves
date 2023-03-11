#pragma once


class Timer
{
	DECLARE_SINGLE(Timer);

public:
	void Init();
	void Update();

	uint32 GetFps() { return _fps; }
	float GetDeltaTime() { return _deltaTime; }

	/*void Wait(int milliseconds) { std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds)); }
	void WaitFrames(int frames, float frameTime)
	{
		int waitTime = static_cast<int>((frames * frameTime) * 1000);
		Wait(waitTime);
	}*/

private:
	uint64	_frequency = 0;
	uint64	_prevCount = 0;
	float	_deltaTime = 0.f;

private:
	uint32	_frameCount = 0;
	float	_frameTime = 0.f;
	uint32	_fps = 0;
};

