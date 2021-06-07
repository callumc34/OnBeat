#define MINIMP3_IMPLEMENTATION

#include <OnBeat/Util/OnSetDetection/OnSetDetection.h>
#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <filesystem>

namespace OnBeat
{

	OnSetFile::OnSetFile()
	{
		reset();
	}

	OnSetFile::OnSetFile(std::string file)
	{
		path = file;
		std::string format = path.substr(path.size() - 3, path.size());
		fileFormat = OnSetFormat::NotLoaded;
		if (format == "mp3")
		{
			loadMp3(file);
		}
		else if (format == "wav")
		{
			loadWav(file);
		}
		else
		{
			fileFormat = OnSetFormat::Unsupported;
		}
	}

	void OnSetFile::reset()
	{
		fileFormat = OnSetFormat::NotLoaded;
		samples = AudioVector{};
		path = "";

		sampleRate = 0;
		lengthSeconds = 0;
		channels = 0;

		wavFile = AudioFile<double>();
		mp3Decoder = {};
		fileInfo = {};
	}

	int OnSetFile::loadMp3(std::string file)
	{
		if (fileFormat != OnSetFormat::NotLoaded)
		{
			return 0;
		}

		path = file;

		if (!std::filesystem::exists(file))
		{
			return 0;
		}

		if (mp3dec_load(&mp3Decoder, path.c_str(), &fileInfo, NULL, NULL))
		{
			fileFormat = OnSetFormat::Error;
			return 0;
		}

		fileFormat = OnSetFormat::MP3;
		sampleRate = fileInfo.hz;
		lengthSeconds = (double)(fileInfo.samples / fileInfo.channels) / fileInfo.hz;
		channels = fileInfo.channels;

		//Add samples to AudioVector
		samples.reserve(fileInfo.channels);
		for (int c = 0; c < fileInfo.channels; c++)
		{
			//Copy buffer data from channel index to next channel
			samples[c] = std::vector<double>(fileInfo.buffer + ((c * fileInfo.samples) / fileInfo.channels),
				fileInfo.buffer + (((c + 1) * fileInfo.samples) / fileInfo.channels));
		}

		free(fileInfo.buffer);

		return 1;
	}

	int OnSetFile::loadWav(std::string file)
	{
		if (fileFormat != OnSetFormat::NotLoaded)
		{
			return 0;
		}

		if (!wavFile.load(file))
		{
			fileFormat = OnSetFormat::Error;
			return 0;
		}

		fileFormat = OnSetFormat::Wave;
		sampleRate = wavFile.getSampleRate();
		lengthSeconds = wavFile.getLengthInSeconds();
		channels = wavFile.getNumChannels();
		samples = wavFile.samples;
		return 1;
	}

	OnSetDetection::OnSetDetection(OnSetOptions options,
		std::string file, int frameSize, int sampleRate)
		: Gist<double>(frameSize, sampleRate), options(options)
	{
		if (std::filesystem::exists(file))
		{
			audioFile = OnSetFile(file);
		}
	}

	AudioVector OnSetDetection::normalise(const AudioVector& beats)
	{
		AudioVector normalised;
		normalised.reserve(beats.size());
		//Normalise each channel
		for (int c = 0; c < beats.size(); c++)
		{
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
				normalised[c][n] = (beats[c][n] - min) / (max - min);
			}

		}

		return normalised;
	}

	AudioVector OnSetDetection::validateAudioVector(const AudioVector& beats)
	{
		return beats;
	}

	int OnSetDetection::createBeatFile(const AudioVector& beats, std::string outputFile, int frameSize, double sampleRate)
	{
		//Todo check for directories and make if needed
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

	double OnSetDetection::findPeakThreshold(const std::vector<double>& beats)
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

	AudioVector OnSetDetection::findBeats(const AudioVector& beats)
	{
		AudioVector beatPoints;
		//Resize for channels
		beatPoints.reserve(beats.size());

		//Loop through channels
		for (int c = 0; c < beats.size(); c++)
		{
			//Resize for all points
			beatPoints[c].reserve(beats[c].size());
			//Calculate threshold and find peaks for each channel
			for (int n = 0; n < beats[c].size(); n++)
			{
				//Calculate mean
				double sum = 0;
				for (int m = n - options.meanWindow; m <= n + options.meanWindow; m++)
				{

					sum += (m > 0 && m < beats[c].size()) ? beats[c][m] : beats[c][0];

				}

				double mean = sum / ((2 * options.meanWindow) + 1);
				//Check point is above mean
				if (beats[c][n] > options.thresholdConstant + (options.thresholdMultiple * mean))
				{
					bool isLocalMaxima = true;
					//Check point is local maxima
					for (int m = n - options.maximaWindow; m < n + options.maximaWindow; m++)
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
						beatPoints[c][n] = beats[c][n];
					}
				}
				else
				{
					beatPoints[c][n] = 0;
				}
			}

		}

		return beatPoints;
	}

	AudioVector OnSetDetection::processAudioVector(const AudioVector& data)
	{
		AudioVector values;
		values.reserve(data.size());
		//Allocate framesize
		std::vector<double> frame;
		frame.reserve(getAudioFrameSize());
		//Processing channels
		for (int c = 0; c < data.size(); c++)
		{
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
		return OnSetDetection::normalise(values);

	}

	AudioVector OnSetDetection::processFile(std::string file)
	{
		if (std::filesystem::exists(file))
		{
			audioFile = OnSetFile(file);
		}

		if (audioFile.getFormat() == OnSetFormat::Error ||
			audioFile.getFormat() == OnSetFormat::NotLoaded ||
			audioFile.getFormat() == OnSetFormat::Unsupported)
		{
			return {};
		}

		//All frames processed in both channels
		return OnSetDetection::normalise(processAudioVector(audioFile.getSamples()));
	}

	OnSetDetection::~OnSetDetection()
	{

	}

}
