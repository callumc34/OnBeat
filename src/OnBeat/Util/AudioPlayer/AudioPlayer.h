#pragma once
#include <FMOD/fmod.hpp>
#include <iostream>

namespace OnBeat
{

	int FMOD_ERRCHECK(FMOD_RESULT r);

	class AudioPlayer
	{
		public:
		AudioPlayer(const char* audioFile = nullptr);

		int pauseAudio(bool pause);
		int playAudio();
		int loadAudio(const char* audioLocation = nullptr);
		int releaseSound();

		bool setVolume(float volume);

		float getVolume();
		const char* getAudioFile();
		unsigned int getCurrentPos();
		unsigned int getLength();
		bool getLoaded();
		bool getPlaying();

		~AudioPlayer();

		FMOD_RESULT result;
		private:
		FMOD::System* system = nullptr;
		FMOD::Sound* sound = nullptr;
		FMOD::ChannelGroup* channelGroup = nullptr;
		FMOD::Channel* channel = nullptr;

		double volume = 1.0;
		int channelID = 0;
		unsigned int length = 0;
		unsigned int time = 0;
		const char* audioFile;

		bool playing = false;
		bool loaded = false;
	};
}