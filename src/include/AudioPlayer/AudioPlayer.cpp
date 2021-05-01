#include <FMOD/fmod_common.h>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_output.h>
#include <AudioPlayer/AudioPlayer.h>
#include <string>
#include <cassert>

#define AP_LOG(x) std::cout << "[Audio Player] " << x << std::endl;
#define AP_WARN(x) std::cerr << "[Audio Player - Warning] " << x << std::endl;


int FMOD_ERRCHECK(FMOD_RESULT r)
{
	//Return/Log error of FMOD
	if (r != FMOD_OK)
	{
		AP_WARN(FMOD_ErrorString(r));

		return 0;
	}
	return 1;
}


int AudioPlayer::pauseAudio(bool pause)
{
	//Set paused state of audio to passed var
	if (FMOD_ERRCHECK(
		channel->setPaused(pause)
	))
	{
		system->update();
		return 0;
	}
	else
	{
		return 1;
	}
}

int AudioPlayer::playAudio()
{
	//Reset audio if already playing
	//Plays audio from audioFile
	//Assigns channel handle
	if (playing)
	{
		if (!FMOD_ERRCHECK(channel->setPosition(0, FMOD_TIMEUNIT_MS)))
		{
			AP_WARN("Error resetting position\n");
			return 0;
		}
		return 1;
	}


	if (!FMOD_ERRCHECK(
		system->playSound(
			sound,
			0,
			false,
			&channel
		)))
	{
		AP_WARN(std::string("Error playing song " + std::string(audioFile) + "\n").c_str());
		return 0;
	}

	channel->setChannelGroup(channelGroup);
	system->update();
	return 1;


}

int AudioPlayer::loadAudio(const char* audioLocation)
{
	//Loads audio into FMOD player
	if (audioLocation == nullptr && audioFile == nullptr)
	{
		AP_WARN("No audio file given\n");
		return 0;
	}

	audioFile = audioLocation;
	FMOD_ERRCHECK(system->createSound(
		audioFile,
		FMOD_DEFAULT,
		0,
		&sound
	));

	AP_LOG(std::string(std::string(audioFile) + "loaded...\n").c_str());
	playing = false;

	loaded = true;

	return 1;

}

int AudioPlayer::releaseSound()
{
	sound->release();
	return 1;
}

bool AudioPlayer::setVolume(float volume)
{
	if (!FMOD_ERRCHECK(channelGroup->setVolume(volume)))
	{
		return false;
	}
	else
	{
		this->volume = volume;
		return true;
	}
}

float AudioPlayer::getVolume()
{
	return volume;
}

const char* AudioPlayer::getAudioFile()
{
	return audioFile;
}

bool AudioPlayer::getPlaying()
{
	if (!FMOD_ERRCHECK(channel->isPlaying(&playing)))
	{
		return playing;
	}
	else
	{
		return false;
	}
}

unsigned int AudioPlayer::getLength()
{
	if (!FMOD_ERRCHECK(sound->getLength(&length, FMOD_TIMEUNIT_MS)))
	{
		return length;
	}
	else
	{
		return 0;
	}
}

bool AudioPlayer::getLoaded()
{
	return loaded;
}

unsigned int AudioPlayer::getCurrentPos()
{
	//Returns the current position in milliseconds
	if (!FMOD_ERRCHECK(channel->isPlaying(&playing)))
	{
		return 0;
	}
	else
	{
		if (!FMOD_ERRCHECK(channel->getPosition(&time, FMOD_TIMEUNIT_MS)))
		{
			return time;
		}
		else
		{
			return 0;
		}
	}
}

AudioPlayer::AudioPlayer(const char* audioLocation)
{
	//Initialises the FMOD system
	audioFile = audioLocation;

	sound = nullptr;

	result = FMOD::System_Create(&system);
	if (!FMOD_ERRCHECK(result)) return;

	result = system->init(32, FMOD_INIT_NORMAL, 0);
	if (!FMOD_ERRCHECK(result)) return;

	result = system->createChannelGroup("AudioPlayer", &channelGroup);
	if (!FMOD_ERRCHECK(result)) return;

	if (audioLocation)
	{
		loadAudio(audioLocation);
	}

	AP_LOG("Initialised");
}

AudioPlayer::~AudioPlayer()
{
	//Stop audio processing
	releaseSound();
	delete channel;
	delete sound;
	delete channel;
}