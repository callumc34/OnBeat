#define MINIMP3_IMPLEMENTATION

#include <OnBeat/Util/OnSetDetection/OnSetDetection.h>
#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <filesystem>

namespace OnBeat
{

	OnSetFile::OnSetFile()
	{
		Reset();
	}

	OnSetFile::OnSetFile(const std::string& file)
	{
		path = file;
		std::string format = path.substr(path.size() - 3, path.size());
		FileFormat = OnSetFormat::NotLoaded;
		if (format == "mp3")
		{
			LoadMP3(file);
		}
		else if (format == "wav")
		{
			LoadWAV(file);
		}
		else
		{
			FileFormat = OnSetFormat::Unsupported;
		}
	}

	void OnSetFile::Reset()
	{
		FileFormat = OnSetFormat::NotLoaded;
		samples = AudioVector{};
		path = "";

		SampleRate = 0;
		LengthSeconds = 0;
		channels = 0;

		WAVFile = AudioFile<double>();
		MP3Decoder = {};
		MP3FileInfo = {};
	}

	int OnSetFile::LoadMP3(const std::string& file)
	{
		if (FileFormat != OnSetFormat::NotLoaded)
		{
			return 0;
		}

		path = file;

		if (!std::filesystem::exists(file))
		{
			return 0;
		}

		if (mp3dec_load(&MP3Decoder, path.c_str(), &MP3FileInfo, NULL, NULL))
		{
			FileFormat = OnSetFormat::Error;
			return 0;
		}

		FileFormat = OnSetFormat::MP3;
		SampleRate = MP3FileInfo.hz;
		LengthSeconds = (double)(MP3FileInfo.samples / MP3FileInfo.channels) / MP3FileInfo.hz;
		channels = MP3FileInfo.channels;

		//Add samples to AudioVector
		samples.reserve(MP3FileInfo.channels);
		for (int c = 0; c < MP3FileInfo.channels; c++)
		{
			//Copy buffer data from channel index to next channel
			samples.push_back(std::vector<double>(MP3FileInfo.buffer + ((c * MP3FileInfo.samples) / MP3FileInfo.channels),
				MP3FileInfo.buffer + (((c + 1) * MP3FileInfo.samples) / MP3FileInfo.channels)));
		}

		free(MP3FileInfo.buffer);

		return 1;
	}

	int OnSetFile::LoadWAV(const std::string& file)
	{
		if (FileFormat != OnSetFormat::NotLoaded)
		{
			return 0;
		}

		if (!WAVFile.load(file))
		{
			FileFormat = OnSetFormat::Error;
			return 0;
		}

		FileFormat = OnSetFormat::Wave;
		SampleRate = WAVFile.getSampleRate();
		LengthSeconds = WAVFile.getLengthInSeconds();
		channels = WAVFile.getNumChannels();
		samples = WAVFile.samples;
		return 1;
	}

	OnSetDetection::OnSetDetection(OnSetOptions options,
		const std::string& file, int frameSize, int sampleRate)
		: Gist<double>(frameSize, sampleRate), options(options)
	{
		if (std::filesystem::exists(file))
		{
			AudioFile = OnSetFile(file);
		}
	}

	AudioVector OnSetDetection::Normalise(const AudioVector& beats)
	{
		AudioVector normalised;
		normalised.reserve(beats.size());
		//Normalise each channel

		std::vector<double> channel;
		channel.reserve(beats[0].size());

		for (int c = 0; c < beats.size(); c++)
		{
			normalised.push_back(channel);

			double max = beats[c][0];
			double min = beats[c][0];
			//Pre pass to get max and min
			for (auto& val : beats[c])
			{
				max = (val > max) ? val : max;
				min = (val < min) ? val : min;
			}
			//New pass for normalised vector
			normalised[c].reserve(beats[c].size());
			for (int n = 0; n < beats[c].size(); n++)
			{
				normalised[c].push_back((beats[c][n] - min) / (max - min));
			}
			channel.clear();
		}

		return normalised;
	}

	AudioVector OnSetDetection::ValidateAudioVector(const AudioVector& beats)
	{
		return beats;
	}

	int OnSetDetection::CreateBeatFile(const AudioVector& beats, const std::string& outputFile, int frameSize, double sampleRate)
	{
		OnBeat::Util::checkPath(outputFile, true);

		std::ofstream outputStream(outputFile);

		if (!outputStream.is_open())
		{
			return 0;
		}
		outputStream << "DEFINE BUFFER=" << frameSize << ";\nDEFINE SAMPLE_RATE=" << sampleRate << ";\n";
		outputStream << "BEGIN BEAT;\n";

		for (int c = 0; c < beats.size(); c++)
		{
			outputStream << "NEW CHANNEL;\n";
			for (int i = 0; i < beats[c].size(); i++)
			{
				outputStream << beats[c][i] << ";\n";
			}
		}

		outputStream << "END BEAT;";

		return 1;
	}

	double OnSetDetection::FindPeakThreshold(const std::vector<double>& beats)
	{

		double sum = 0;
		int size = (int)beats.size();
		for (auto& beat : beats)
		{
			if (beat == 0)
			{
				size -= 1;
			}
			else
			{
				sum += beat;
			}
		}

		return sum / size;
	}

	AudioVector OnSetDetection::FindBeats(const AudioVector& beats)
	{
		AudioVector beatPoints;
		//Resize for channels
		beatPoints.reserve(beats.size());

		std::vector<double> channel;
		channel.reserve(beats[0].size());

		//Loop through channels
		for (int c = 0; c < beats.size(); c++)
		{
			beatPoints.push_back(channel);
			//Resize for all points
			beatPoints[c].reserve(beats[c].size());
			//Calculate threshold and find peaks for each channel
			for (int n = 0; n < beats[c].size(); n++)
			{
				//Calculate mean
				double sum = 0;
				for (int m = n - options.MeanWindow; m <= n + options.MeanWindow; m++)
				{

					sum += (m > 0 && m < beats[c].size()) ? beats[c][m] : beats[c][0];

				}

				double mean = sum / ((2 * options.MeanWindow) + 1);
				//Check point is above mean
				if (beats[c][n] > options.ThresholdConstant + (options.ThresholdMultiple * mean))
				{
					bool isLocalMaxima = true;
					//Check point is local maxima
					for (int m = n - options.MaximaWindow; m < n + options.MaximaWindow; m++)
					{
						//Ensure m is a valid index else m is looped around
						m = (m < 0) ? m = 0 - m : (m >= beats[c].size()) ? m = m - (int)beats[c].size() : m;
						if (beats[c][n] < beats[c][m])
						{
							isLocalMaxima = false;
							break;
						}
					}
					if (isLocalMaxima)
					{
						beatPoints[c].push_back(beats[c][n]);
					}
					else
					{
						beatPoints[c].push_back(0);
					}
				}
				else
				{
					beatPoints[c].push_back(0);
				}
			}
			channel.clear();

		}

		return beatPoints;
	}

	AudioVector OnSetDetection::ProcessAudioVector(const AudioVector& data)
	{
		AudioVector values;
		values.reserve(data.size());
		//Allocate framesize
		std::vector<double> frame;
		frame.reserve(getAudioFrameSize());
		//Processing channels
		for (int c = 0; c < data.size(); c++)
		{
			values.push_back(frame);
			//Loop through each frame
			for (int i = getAudioFrameSize(); i < data[c].size(); i += getAudioFrameSize())
			{
				for (int buf = i - getAudioFrameSize(); buf < i; buf++)
				{
					frame.push_back(data[c][buf]);
				}

				processAudioFrame(frame);

				values[c].push_back(spectralDifference());
				frame.clear();
			}
		}

		//All frames processed in both channels
		return OnSetDetection::Normalise(values);

	}

	AudioVector OnSetDetection::ProcessFile(const std::string& file)
	{
		if (std::filesystem::exists(file))
		{
			AudioFile = OnSetFile(file);
		}

		if (AudioFile.GetFormat() == OnSetFormat::Error ||
			AudioFile.GetFormat() == OnSetFormat::NotLoaded ||
			AudioFile.GetFormat() == OnSetFormat::Unsupported)
		{
			return {};
		}

		//All frames processed in both channels
		return ProcessAudioVector(AudioFile.GetSamples());
	}

	OnSetDetection::~OnSetDetection()
	{

	}

}
