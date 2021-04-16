#pragma once
#include <FMOD/fmod.hpp>
#include <iostream>

int FMOD_ERRCHECK(FMOD_RESULT r, bool throwE = false);

class AudioPlayer
{
	public:
		AudioPlayer(const char* audioFile = nullptr);

		int pauseAudio(bool pause);
		int playAudio();
		int loadAudio(const char* audioLocation = nullptr);
		int releaseSound();

		const char* getAudioFile();
		unsigned int getCurrentPos();
		unsigned int getLength();
		bool getLoaded();
		bool getPlaying();

		~AudioPlayer();

		FMOD_RESULT result;
	private:
		FMOD::System* system;
		FMOD::Sound* sound;
		FMOD::Channel* channel = 0;
		int channelID = 0;
		unsigned int length = 0;
		unsigned int time = 0;
		const char* audioFile;
		bool playing = false;
		bool loaded = false;
};
