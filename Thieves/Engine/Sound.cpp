#include "pch.h"
#include "Sound.h"

void Sound::Init()
{
	FMOD_System_Create(&System, FMOD_VERSION);
	FMOD_System_Init(System, CHANNEL_COUNT, FMOD_INIT_NORMAL, NULL);


	// bgmList
	FMOD_System_CreateSound(System, "..\\Resources\\Sound\\Unity.mp3", FMOD_LOOP_NORMAL, 0, &_bgmSound[0]);
}

void Sound::PlaySound(int bgmNum, int channelNum)
{
	if(bgmNum < SOUND_COUNT && channelNum < CHANNEL_COUNT )
		FMOD_System_PlaySound(System, _bgmSound[bgmNum], NULL, 0, &_channel[channelNum]);
}

void Sound::SetVolume(int channelNum, float volume)
{
	FMOD_Channel_SetVolume(_channel[channelNum], volume);
}
