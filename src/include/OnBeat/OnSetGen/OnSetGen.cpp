#include <OnBeat/OnSetGen/OnSetGen.h>
#include <filesystem>

OnSetGen::OnSetGen(double frameSize, double sampleRate)
	: Gist<double>(frameSize, sampleRate)
{
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
		std::cerr << "Error opening file " << outputFile << std::endl;
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

	std::cout << "File written to " << outputFile << std::endl;

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

OnSetGen::~OnSetGen()
{

}