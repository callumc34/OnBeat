#pragma once
#include <gist/Gist.h>
#include <gist/AudioFile.h>

//Define AudioVector as a 2d vector of doubles
typedef std::vector<std::vector<double>> AudioVector;

class OnSetGen : public Gist<double>
{
	public:
		OnSetGen(double thresholdC, double thresholdM, int meanW, int maximaW,
			double frameSize = 512, double sampleRate = 44100);
		~OnSetGen();


		//Peak detection algorithm
		AudioVector findBeats(AudioVector beats);
		
		int loadAudioFile(const char* file);

		//Gist onset detection of a .wav file using spectralDifference
		int createBeatFile(AudioVector beats, const char* outputFile);

		AudioVector processFile(const char* file = nullptr);

		//Get private values
		double getThresholdConstant() { return thresholdConstant; }
		double getThresholdMultiple() { return thresholdMultiple; }
		int getMeanWindow() { return meanWindow; }
		int getMaximaWindow() { return maximaWindow; }


		//Set private values
		int setThresholdValues(double thresholdConstant = NULL, double thresholdMultiple = NULL, 
			int meanW = NULL, int maximaW = NULL);

	private:
		//Threshold values
		double thresholdConstant;
		double thresholdMultiple;
		int meanWindow;
		int maximaWindow;

		//AudioFile currently wav only
		AudioFile<double> audioFile;

};