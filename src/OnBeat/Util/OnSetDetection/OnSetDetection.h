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
			OnSetFile(const std::string& file);

			void reset();

			int loadMp3(const std::string& file);
			int loadWav(const std::string& file);

			const AudioVector& getSamples() const { return samples; }
			const std::string& getPath() const { return path; }
			OnSetFormat getFormat() const { return fileFormat; }
			double getSamplingFrequency() const { return sampleRate; }
			double getLengthInSeconds() const { return lengthSeconds; }
			int getChannels() const { return channels; }

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
				const std::string& file = "",	int frameSize = 512, int sampleRate = 44100);
			~OnSetDetection();

			static AudioVector normalise(const AudioVector& beats);
			static AudioVector validateAudioVector(const AudioVector& beats);
			static int createBeatFile(const AudioVector& beats, const std::string& outputFile, int frameSize = 512, double sampleRate = 44100);
			static double findPeakThreshold(const std::vector<double>& beats);


			//Peak detection algorithm
			AudioVector findBeats(const AudioVector& beats);

			//Gist onset detection of a .wav file using spectralDifference
			AudioVector processFile(const std::string& file = "");
			AudioVector processAudioVector(const AudioVector& data);

			//Get private values
			const OnSetOptions& getOptions() const { return options; }
			const OnSetFile& getAudioFile() const { return audioFile; }

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