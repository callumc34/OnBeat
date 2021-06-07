#pragma once
#include <FMOD/fmod.hpp>
#include <iostream>

namespace OnBeat
{

	int FMOD_ERRCHECK(FMOD_RESULT r);

	class AudioPlayer
	{
		public:
			AudioPlayer(const std::string& audioFile = "");
			~AudioPlayer();

			int PauseAudio(bool pause);
			int PlayAudio();
			int LoadAudio(const std::string& audioLocation = "");
			int ReleaseSound();

			bool SetVolume(float volume);

			float GetVolume();
			const std::string& GetAudioFile();
			unsigned int GetCurrentPos();
			unsigned int GetLength();
			bool GetLoaded();
			bool GetPlaying();


			FMOD_RESULT result;
		private:
			FMOD::System* system = nullptr;
			FMOD::Sound* sound = nullptr;
			FMOD::ChannelGroup* ChannelGroup = nullptr;
			FMOD::Channel* channel = nullptr;

			double volume = 1.0;
			int ChannelID = 0;
			unsigned int length = 0;
			unsigned int time = 0;
			std::string AudioFile;

			bool playing = false;
			bool loaded = false;
	};
}