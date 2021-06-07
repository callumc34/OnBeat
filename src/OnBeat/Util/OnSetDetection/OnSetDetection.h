#pragma once
#include <AudioFile/AudioFile.h>
#include <minimp3/minimp3_ex.h>
#include <Gist.h>


namespace OnBeat {
	//Define AudioVector as a 2d vector of doubles
	typedef std::vector<std::vector<double>> AudioVector;

	struct OnSetOptions
	{
		double ThresholdConstant;
		double ThresholdMultiple;
		int MeanWindow;
		int MaximaWindow;
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

			void Reset();

			int LoadMP3(const std::string& file);
			int LoadWAV(const std::string& file);

			const AudioVector& GetSamples() const { return samples; }
			const std::string& GetPath() const { return path; }
			OnSetFormat GetFormat() const { return FileFormat; }
			double GetSamplingFrequency() const { return SampleRate; }
			double GetLengthInSeconds() const { return LengthSeconds; }
			int GetChannels() const { return channels; }

		private:
			OnSetFormat FileFormat;
			AudioVector samples;

			std::string path;

			double SampleRate;
			double LengthSeconds;
			int channels;

			//Wav values
			AudioFile<double> WAVFile;

			//Mp3 values
			mp3dec_t MP3Decoder;
			mp3dec_file_info_t MP3FileInfo;
	};

	class OnSetDetection : public Gist<double>
	{
		public:
			OnSetDetection(OnSetOptions options,
				const std::string& file = "",	int frameSize = 512, int sampleRate = 44100);
			~OnSetDetection();

			static AudioVector Normalise(const AudioVector& beats);
			static AudioVector ValidateAudioVector(const AudioVector& beats);
			static int CreateBeatFile(const AudioVector& beats, const std::string& outputFile, int frameSize = 512, double sampleRate = 44100);
			static double FindPeakThreshold(const std::vector<double>& beats);


			//Peak detection algorithm
			AudioVector FindBeats(const AudioVector& beats);

			//Gist onset detection of a .wav file using spectralDifference
			AudioVector ProcessFile(const std::string& file = "");
			AudioVector ProcessAudioVector(const AudioVector& data);

			//Get private values
			const OnSetOptions& GetOptions() const { return options; }
			const OnSetFile& GetAudioFile() const { return AudioFile; }

			void SetOptions(OnSetOptions options)
			{
				this->options = options;
			}

			void SetAudioFile(OnSetFile audioFile)
			{
				this->AudioFile = audioFile;
			}

		private:
			//Threshold values
			//Recommended 0 1 8 8
			OnSetOptions options;

			OnSetFile AudioFile;

	};
}