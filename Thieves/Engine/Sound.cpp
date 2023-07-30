#include "pch.h"
#include "Sound.h"

void Sound::Init()
{
	FMOD_System_Create(&System, FMOD_VERSION);
	FMOD_System_Init(System, CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL);


	// bgm List
	FMOD_System_CreateSound(System, "..\\Resources\\Sound\\Unity.mp3", FMOD_LOOP_NORMAL, 0, &_bgmSound[0]);

	// effectsound List
	FMOD_System_CreateSound(System, "..\\Resources\\Sound\\hit.mp3", FMOD_DEFAULT, 0, &_effectSound[0]);
	FMOD_System_CreateSound(System, "..\\Resources\\Sound\\trap.mp3", FMOD_DEFAULT, 0, &_effectSound[1]);
}

void Sound::PlaySound(int bgmNum, int channelNum)
{
	if(bgmNum < SOUND_COUNT && channelNum < CHANNEL_COUNT )
		FMOD_System_PlaySound(System, _bgmSound[bgmNum], NULL, 0, &_channel[channelNum]);
}

void Sound::PlayEffectSound(int effectNum, int channelNum)
{
	if (effectNum < EFFECT_COUNT && channelNum < CHANNEL_COUNT)
		FMOD_System_PlaySound(System, _effectSound[effectNum], NULL, 0, &_channel[channelNum]);
}

void Sound::SetVolume(int channelNum, float volume)
{
	FMOD_Channel_SetVolume(_channel[channelNum], volume);
}

void Sound::PauseSound(int channelNum, bool flag)
{
	FMOD_Channel_SetPaused(_channel[channelNum], flag);
}

void Sound::StopSound(int channelNum)
{
	FMOD_Channel_Stop(_channel[channelNum]);
}
