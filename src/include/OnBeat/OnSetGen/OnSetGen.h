#pragma once
#include <Gist.h>
#include <AudioFile/AudioFile.h>

#include <minimp3/minimp3_ex.h>

//Define AudioVector as a 2d vector of doubles
typedef std::vector<std::vector<double>> AudioVector;

enum class OnSetFormat
{
	Error,
	NotLoaded,
	Unsupported,
	Wave,
	Aiff,
	MP3
};

class OnSetFile
{
	public:
		OnSetFile();
		OnSetFile(const char* file);

		void reset();

		int loadMp3(const char* file);
		int loadWav(const char* file);

		std::string getPath() { return path; }
		OnSetFormat getFormat() { return fileFormat; }
		AudioVector getSamples() { return samples; }
		double getSamplingFrequency() { return sampleRate; }
		double getLengthInSeconds() { return lengthSeconds; }
		int getChannels() { return channels; }

	private:
		OnSetFormat fileFormat;
		AudioVector samples;

		std::string path;

		double sampleRate;
		double lengthSeconds;
		int channels;

		//Wav values
		AudioFile<double> wavFile;

		//Mp3 values
		mp3dec_t mp3Decoder;
		mp3dec_file_info_t fileInfo;
};

class OnSetGen : public Gist<double>
{
	public:
		OnSetGen(double thresholdC, double thresholdM, int meanW, int maximaW,
			const char* file = nullptr,	double frameSize = 512, double sampleRate = 44100);
		~OnSetGen();

		static AudioVector normalise(AudioVector beats);
		static AudioVector validateAudioVector(AudioVector beats);
		static int createBeatFile(AudioVector beats, const char* outputFile, int frameSize = 512, double sampleRate = 44100);
		static double findPeakThreshold(std::vector<double> beats);


		//Peak detection algorithm
		AudioVector findBeats(AudioVector beats);

		//Gist onset detection of a .wav file using spectralDifference
		AudioVector processFile(const char* file = nullptr);
		AudioVector processAudioVector(AudioVector data);

		//Get private values
		double getThresholdConstant() { return thresholdConstant; }
		double getThresholdMultiple() { return thresholdMultiple; }
		int getMeanWindow() { return meanWindow; }
		int getMaximaWindow() { return maximaWindow; }
		OnSetFile getAudioFile() { return audioFile; }


		//Set private values
		void setThresholdValues(double thresholdConstant = NULL, double thresholdMultiple = NULL, 
			int meanW = NULL, int maximaW = NULL);
		void setAudioFile(OnSetFile af)
		{
			audioFile = af;
		}

	private:
		//Threshold values
		//Recommended 0 1 8 3
		double thresholdConstant;
		double thresholdMultiple;
		int meanWindow;
		int maximaWindow;

		//AudioFile currently wav only
		OnSetFile audioFile;

};