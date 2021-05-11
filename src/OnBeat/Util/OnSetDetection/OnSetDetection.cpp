#define MINIMP3_IMPLEMENTATION

#include <OnBeat/Util/OnSetDetection/OnSetDetection.h>
#include <filesystem>

namespace OnBeat
{

	OnSetFile::OnSetFile()
	{
		reset();
	}

	OnSetFile::OnSetFile(const char* file)
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

	int OnSetFile::loadMp3(const char* file)
	{
		if (fileFormat != OnSetFormat::NotLoaded)
		{
			return 0;
		}

		path = file;

		if (mp3dec_load(&mp3Decoder, path.c_str(), &fileInfo, NULL, NULL))
		{
			fileFormat = OnSetFormat::Error;
			return 0;
		}

		fileFormat = OnSetFormat::MP3;
		sampleRate = fileInfo.hz;
		lengthSeconds = (fileInfo.samples / fileInfo.channels) / fileInfo.hz;
		channels = fileInfo.channels;

		//Add samples to AudioVector
		for (int c = 0; c < fileInfo.channels; c++)
		{
			samples.push_back(std::vector<double>());
			for (int n = 0; n < fileInfo.samples / channels; n++)
			{
				samples[c].push_back(fileInfo.buffer[(c * fileInfo.channels) + n]);
			}
		}

		free(fileInfo.buffer);

		return 1;
	}

	int OnSetFile::loadWav(const char* file)
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

	OnSetDetection::OnSetDetection(double thresholdC, double thresholdM, int meanW, int maximaW,
		const char* file, double frameSize, double sampleRate)
		: Gist<double>(frameSize, sampleRate)
	{
		thresholdConstant = thresholdC;
		thresholdMultiple = thresholdM;
		meanWindow = meanW;
		maximaWindow = maximaW;
		if (file != nullptr)
		{
			audioFile = OnSetFile(file);
		}
	}

	AudioVector OnSetDetection::normalise(AudioVector beats)
	{
		AudioVector normalised;
		normalised.resize(beats.size());
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
			normalised[c].resize(beats[c].size());
			for (int n = 0; n < beats[c].size(); n++)
			{
				normalised[c][n] = (beats[c][n] - min) / (max - min);
			}

		}

		return normalised;
	}

	AudioVector OnSetDetection::validateAudioVector(AudioVector beats)
	{
		return beats;
	}

	int OnSetDetection::createBeatFile(AudioVector beats, const char* outputFile, int frameSize, double sampleRate)
	{
		//Todo check for directories and make if needed

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

	double OnSetDetection::findPeakThreshold(std::vector<double> beats)
	{

		double sum = 0;
		int size = beats.size();
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

	AudioVector OnSetDetection::findBeats(AudioVector beats)
	{
		AudioVector beatPoints;
		//Resize for channels
		beatPoints.resize(beats.size());

		//Loop through channels
		for (int c = 0; c < beats.size(); c++)
		{
			//Resize for all points
			beatPoints[c].resize(beats[c].size());
			//Calculate threshold and find peaks for each channel
			for (int n = 0; n < beats[c].size(); n++)
			{
				//Calculate mean
				double sum = 0;
				for (int m = n - meanWindow; m <= n + meanWindow; m++)
				{

					sum += (m > 0 && m < beats[c].size()) ? beats[c][m] : beats[c][0];

				}

				double mean = sum / ((2 * meanWindow) + 1);
				//Check point is above mean
				if (beats[c][n] > thresholdConstant + (thresholdMultiple * mean))
				{
					bool isLocalMaxima = true;
					//Check point is local maxima
					for (int m = n - maximaWindow; m <= n + maximaWindow; m++)
					{
						m = (m < 0) ? 0 : m;
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

	AudioVector OnSetDetection::processAudioVector(AudioVector data)
	{
		AudioVector values;

		//Processing channels
		for (int c = 0; c < data.size(); c++)
		{
			values.push_back(std::vector<double>());
			std::vector<double> frame;

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

	AudioVector OnSetDetection::processFile(const char* file)
	{
		if (file)
		{
			OnSetFile audioFile(file);
		}

		if (audioFile.getFormat() == OnSetFormat::Error ||
			audioFile.getFormat() == OnSetFormat::NotLoaded ||
			audioFile.getFormat() == OnSetFormat::Unsupported)
		{
			return {};
		}

		AudioVector fileSamples = audioFile.getSamples();

		//All frames processed in both channels
		return OnSetDetection::normalise(processAudioVector(fileSamples));

	}

	void OnSetDetection::setThresholdValues(double thresholdC, double thresholdM, int meanW, int maximaW)
	{
		thresholdConstant = (thresholdC == NULL) ? thresholdConstant : thresholdC;
		thresholdMultiple = (thresholdM == NULL) ? thresholdMultiple : thresholdM;
		meanWindow = (meanW == NULL) ? meanWindow : meanW;
		maximaW = (maximaW == NULL) ? maximaWindow : maximaW;
	}

	OnSetDetection::~OnSetDetection()
	{

	}

}