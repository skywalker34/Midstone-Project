#include "AudioManager.h"

bool AudioManager::OnCreate()
{
	SoundEngine->setSoundVolume(0.4f);
	//SoundEngine->play2D("audio/BackGroundMusic2.mp3", true); // Audio For Game 
	return true;
}

void AudioManager::OnDestroy()
{
	SoundEngine->drop();
}

void AudioManager::PlaySound2D(std::string name)
{
}

void AudioManager::PlaySound3D(std::string name, Vec3 pos)
{
}

void AudioManager::PlaySound(std::string name)
{
	if (name == "Planet_Hit")
	{
	}
}
