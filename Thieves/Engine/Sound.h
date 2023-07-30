#pragma once


class Sound
{
public:
	void Init();

	void PlaySound(int bgmNum, int channelNum);
	void SetVolume(int channelNum, float volume);

private:
	FMOD_SYSTEM* System;
	FMOD_SOUND* _bgmSound[SOUND_COUNT];
	FMOD_SOUND* _effectSound[EFFECT_COUNT];
	FMOD_CHANNEL* _channel[CHANNEL_COUNT];
};

