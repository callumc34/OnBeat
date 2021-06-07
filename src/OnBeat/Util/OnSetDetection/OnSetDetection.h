#pragma once
#include <AudioFile/AudioFile.h>
#include <minimp3/minimp3_ex.h>
#include <Gist.h>


namespace OnBeat {
	//Define AudioVector as a 2d vector of doubles
	typedef std::vector<std::vector<double>> AudioVector;

	struct OnSetOptions
	{
		double thresholdConstant;
		double thresholdMultiple;
		int meanWindow;
		int maximaWindow;
	};

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
			OnSetFile(std::string file);

			void reset();

			int loadMp3(std::string file);
			int loadWav(std::string file);

			AudioVector getSamples() { return samples; }
			std::string getPath() { return path; }
			OnSetFormat getFormat() { return fileFormat; }
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

	class OnSetDetection : public Gist<double>
	{
		public:
			OnSetDetection(OnSetOptions options,
				std::string file = "",	int frameSize = 512, int sampleRate = 44100);
			~OnSetDetection();

			static AudioVector normalise(const AudioVector& beats);
			static AudioVector validateAudioVector(const AudioVector& beats);
			static int createBeatFile(const AudioVector& beats, std::string outputFile, int frameSize = 512, double sampleRate = 44100);
			static double findPeakThreshold(const std::vector<double>& beats);


			//Peak detection algorithm
			AudioVector findBeats(const AudioVector& beats);

			//Gist onset detection of a .wav file using spectralDifference
			AudioVector processFile(std::string file = "");
			AudioVector processAudioVector(const AudioVector& data);

			//Get private values
			OnSetOptions getOptions() { return options; }
			OnSetFile getAudioFile() { return audioFile; }

			void setOptions(OnSetOptions options)
			{
				this->options = options;
			}

			void setAudioFile(OnSetFile audioFile)
			{
				this->audioFile = audioFile;
			}

		private:
			//Threshold values
			//Recommended 0 1 8 8
			OnSetOptions options;

			//AudioFile currently wav only
			OnSetFile audioFile;

	};
}