#pragma once
#include <gist/Gist.h>
#include <gist/AudioFile.h>

typedef std::vector<std::vector<double>> AudioVector;

class OnSetGen : public Gist<double>
{
	public:
		OnSetGen(double frameSize = 512, double sampleRate = 44100);
		~OnSetGen();
		
		int loadAudioFile(const char* file);

		int createBeatFile(AudioVector beats, const char* outputFile);

		AudioVector processFile(const char* file = nullptr);

	private:
		AudioFile<double> audioFile;

};