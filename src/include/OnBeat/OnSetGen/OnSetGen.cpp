#include <OnBeat/OnSetGen/OnSetGen.h>
#include <filesystem>

OnSetGen::OnSetGen(double thresholdC, double thresholdM, int meanW, int maximaW,
	double frameSize, double sampleRate)
	: Gist<double>(frameSize, sampleRate)
{
	thresholdConstant = thresholdC;
	thresholdMultiple = thresholdM;
	meanWindow = meanW;
	maximaWindow = maximaW;
}

AudioVector OnSetGen::normalise(AudioVector beats)
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

AudioVector OnSetGen::validateAudioVector(AudioVector beats)
{
	return beats;
}

int OnSetGen::createBeatFile(AudioVector beats, const char* outputFile, int frameSize, double sampleRate)
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

AudioVector OnSetGen::findBeats(AudioVector beats)
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
					beatPoints[c][n] = 1;
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

int OnSetGen::loadAudioFile(const char* file)
{
	audioFile.load(file);
	return 1;
}

AudioVector OnSetGen::processFile(const char* file)
{
	AudioVector values;
	if (file == nullptr)
	{
		if (audioFile.getFileFormat() == AudioFileFormat::NotLoaded)
		{
			//Error;
			return values;
		}
	}
	else
	{
		audioFile.load(file);
	}

	std::vector<double> frame;

	//Processing channels
	for (int c = 0; c < audioFile.samples.size(); c++)
	{
		std::vector<double> channel;
		values.push_back(channel);

		//Loop through each frame
		for (int i = getAudioFrameSize(); i < audioFile.samples[c].size(); i += getAudioFrameSize())
		{
			for (int buf = i - getAudioFrameSize(); buf < i; buf++)
			{
				frame.push_back(audioFile.samples[c][buf]);
			}

			processAudioFrame(frame);

			values[c].push_back(spectralDifference());
			frame.clear();
		}

		channel.clear();
	}

	//All frames processed in both channels
	return OnSetGen::normalise(values);

}

int OnSetGen::setThresholdValues(double thresholdC, double thresholdM, int meanW, int maximaW)
{
	thresholdConstant = (thresholdC == NULL) ? thresholdConstant : thresholdC;
	thresholdMultiple = (thresholdM == NULL) ? thresholdMultiple : thresholdM;
	meanWindow = (meanW == NULL) ? meanWindow : meanW;
	maximaW = (maximaW == NULL) ? maximaWindow : maximaW;

	return 1;
}

OnSetGen::~OnSetGen()
{

}


