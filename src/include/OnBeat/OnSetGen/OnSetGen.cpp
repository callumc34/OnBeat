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

AudioVector OnSetGen::findBeats(AudioVector beats)
{
	AudioVector beatPoints;
	
	//Loop through channels
	for (int c = 0; c < beats.size(); c++)
	{
		std::vector<double> beatChannel;
		beatChannel.resize(beats[c].size());
		//Calculate threshold and find peaks for each channel
		double threshold = 0;
		for (int n = 0; n < beats[c].size(); n++)
		{
			//Calculate mean
			double sum = 0;
			for (int m = n - meanWindow; m <= n + meanWindow; m++)
			{

				sum += (m > 0) ? beats[c][m] : beats[c][0];
				
			}

			double mean = sum / meanWindow;

			//Check point is above mean
			if (beats[c][n] > mean)
			{
				bool isLocalMaxima = true;
				//Check point is local maxima
				for (int m = n - maximaWindow; m <= n + maximaWindow; m++)
				{

					if (beats[c][n] < beats[c][m])
					{
						isLocalMaxima = false;
						break;
					}
				}
				if (isLocalMaxima)
				{
					beatChannel[n] = 1;
				}
			}
			else
			{
				beatChannel[n] = 0;
			}
		}

		beatPoints.push_back(beatChannel);

	}

	return beatPoints;
}

int OnSetGen::loadAudioFile(const char* file)
{
	audioFile.load(file);
	return 1;
}

int OnSetGen::createBeatFile(AudioVector beats, const char* outputFile)
{
	//Todo check for directories and make if needed

	std::ofstream outputStream(outputFile);

	if (!outputStream.is_open())
	{
		return 0;
	}
	outputStream << "DEFINE BUFFER=" << getAudioFrameSize() << ";\nDEFINE SAMPLE_RATE=" << getSamplingFrequency() << ";\n";
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

AudioVector OnSetGen::processFile(const char* file)
{
	AudioVector sdValues;
	if (file == nullptr)
	{
		if (audioFile.getFileFormat() == AudioFileFormat::NotLoaded)
		{
			//Error;
			return sdValues;
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
		sdValues.push_back(channel);

		//Loop through each frame
		for (int i = getAudioFrameSize(); i < audioFile.samples[c].size(); i += getAudioFrameSize())
		{
			for (int buf = i - getAudioFrameSize(); buf < i; buf++)
			{
				frame.push_back(audioFile.samples[c][buf]);
			}

			processAudioFrame(frame);

			sdValues[c].push_back(spectralDifference());
			frame.clear();
		}

		channel.clear();
	}

	//All frames processed in both channels
	return sdValues;

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