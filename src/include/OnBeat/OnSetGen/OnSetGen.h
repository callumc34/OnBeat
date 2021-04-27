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

		static AudioVector normalise(AudioVector beats);
		static AudioVector validateAudioVector(AudioVector beats);
		static int createBeatFile(AudioVector beats, const char* outputFile, int frameSize = 512, double sampleRate = 44100);
		static double findPeakThreshold(std::vector<double> beats);


		//Peak detection algorithm
		AudioVector findBeats(AudioVector beats);
		
		int loadAudioFile(const char* file);

		//Gist onset detection of a .wav file using spectralDifference

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
		//Recommended 0 1 8 3
		double thresholdConstant;
		double thresholdMultiple;
		int meanWindow;
		int maximaWindow;

		//AudioFile currently wav only
		AudioFile<double> audioFile;

};