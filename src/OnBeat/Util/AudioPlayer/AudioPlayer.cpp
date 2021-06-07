#include <FMOD/fmod_common.h>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_output.h>
#include <OnBeat/Util/AudioPlayer/AudioPlayer.h>
#include <string>
#include <cassert>

#define AP_LOG(x) std::cout << "[Audio Player] " << x << std::endl;
#define AP_WARN(x) std::cerr << "[Audio Player - Warning] " << x << std::endl;

namespace OnBeat
{

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


	int AudioPlayer::PauseAudio(bool pause)
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

	int AudioPlayer::PlayAudio()
	{
		//Reset audio if already playing
		//Plays audio from AudioFile
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
			AP_WARN(std::string("Error playing song " + std::string(AudioFile) + "\n").c_str());
			return 0;
		}

		channel->setChannelGroup(ChannelGroup);
		system->update();
		return 1;


	}

	int AudioPlayer::LoadAudio(const std::string& audioLocation)
	{
		//Loads audio into FMOD player
		if (audioLocation.empty() && AudioFile.empty())
		{
			AP_WARN("No audio file given\n");
			return 0;
		}

		AudioFile = audioLocation;
		FMOD_ERRCHECK(system->createSound(
			AudioFile.c_str(),
			FMOD_DEFAULT,
			0,
			&sound
		));

		AP_LOG(std::string(std::string(AudioFile) + " loaded...\n").c_str());
		playing = false;

		loaded = true;

		return 1;

	}

	int AudioPlayer::ReleaseSound()
	{
		sound->release();
		return 1;
	}

	bool AudioPlayer::SetVolume(float volume)
	{
		if (!FMOD_ERRCHECK(ChannelGroup->setVolume(volume)))
		{
			return false;
		}
		else
		{
			this->volume = volume;
			return true;
		}
	}

	float AudioPlayer::GetVolume()
	{
		return (float)volume;
	}

	const std::string& AudioPlayer::GetAudioFile()
	{
		return AudioFile;
	}

	bool AudioPlayer::GetPlaying()
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

	unsigned int AudioPlayer::GetLength()
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

	bool AudioPlayer::GetLoaded()
	{
		return loaded;
	}

	unsigned int AudioPlayer::GetCurrentPos()
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

	AudioPlayer::AudioPlayer(const std::string& audioLocation)
	{
		//Initialises the FMOD system
		AudioFile = audioLocation;

		sound = nullptr;

		result = FMOD::System_Create(&system);
		if (!FMOD_ERRCHECK(result)) return;

		result = system->init(32, FMOD_INIT_NORMAL, 0);
		if (!FMOD_ERRCHECK(result)) return;

		result = system->createChannelGroup("AudioPlayer", &ChannelGroup);
		if (!FMOD_ERRCHECK(result)) return;

		if (!audioLocation.empty())
		{
			LoadAudio(audioLocation);
		}

		AP_LOG("Initialised");
	}

	AudioPlayer::~AudioPlayer()
	{
		//Stop audio processing
		ReleaseSound();
		delete channel;
		delete sound;
		delete channel;
	}
}