#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <AudioBeatGen/AudioBeatGen.h>
#include <sys/stat.h>

bool fileExists(const char* file) {
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

int AudioBeat::createBeatFile(AudioVector beats, const char* outputDir) {
	const char* outputFile;
	std::ofstream outputStream(outputFile);

	assert(outputStream.is_open());

	AudioVector frames = processFrames();

	for (int c = 0; c < frames.size(); c++) {
		for (int i = 0; i < frames[c].size(); i++) {

		}
	}

	return 1;
}

AudioVector AudioBeat::cleanUpBeats(AudioVector beats, const char* logFile) {
	bool log;
	std::fstream logStream(logFile, std::fstream::app);

	if (!logStream.is_open()) {
		std::cout << "Could not open log file...\nContinuing without logging...\n";
		log = false;
	}
	else {
		log = true;
	}
	double beatSize;
	for (int c = 0; c < beats.size(); c++) {
		logStream << "Channel " << c + 1 << std::endl;
		for (int i = 0; i < beats[c].size(); i++) {
			if (beats[c][i] < beats[c][i + 1]) {
				beats[c][i] = 0;
			}

			logStream << "SD Beat: " << beats[c][i] << std::endl;
		}
	}

	return beats;
}

AudioVector AudioBeat::processFrames(const char* logFile, AudioBeatFlags::Value method) {

	const double THRESHOLD_MULTIPLIER = 1.2;

	if (audioFile.getFileFormat() == AudioFileFormat::NotLoaded) {
		throw std::runtime_error("Audio file not loaded");
	}

	bool log;
	std::ofstream logStream(logFile);

	if (!logStream.is_open()) {
		std::cout << "Could not open log file...\nContinuing without logging...\n";
		log = false;
	} else {
		log = true;
	}

	AudioVector beats;

	std::vector<double> frame;

	for (int c = 0; c < audioFile.samples.size(); c++) {
		std::cout << "Processing channel: " << c + 1 << std::endl;

		std::vector<double> channel;
		beats.push_back(channel);

		double threshold = 0;

		for (int i = getAudioFrameSize(); i < audioFile.samples[c].size(); i += getAudioFrameSize()) {
			
			for (int buf = i - getAudioFrameSize(); buf < i; buf++) {
				frame.push_back(audioFile.samples[c][buf]);
			}
			
			processAudioFrame(frame);

			double dif = method == AudioBeatFlags::SPECTRAL_DIFFERENCE ? spectralDifference() : energyDifference();

			threshold += dif * THRESHOLD_MULTIPLIER;

			double currentThreshold = threshold / beats[c].size() + 1;

			beats[c].push_back(dif > currentThreshold ? dif : 0);


			if (log) {
				logStream << "Audio frame [" << i / getAudioFrameSize() << "] of [" << audioFile.samples[c].size() / getAudioFrameSize() << "]\n";
				logStream << "Difference: " << dif
					<< "\nThreshold: " << currentThreshold;
				logStream << std::endl;
				

			}

			frame.clear();
		}

		channel.clear();

	}

	if (log) {
		logStream.close();
	}

	//Vectors must be of equal size for time to correspond to difference
	assert(beats[0].size() ==
		audioFile.samples[0].size() / getAudioFrameSize());

	if (beats.size() > 1) {
		for (int i = 1; i < beats.size() - 1; i++)
			assert(beats[i].size() == beats[-1].size());
	}

	std::cout << "Finished processing frames\n";
	
	return cleanUpBeats(beats);
}

int AudioBeat::loadAudio(const char* file) {
	//Load audio using AudioFile.h to process the frames
	std::cout << "Loading file: " << file << "...\n";
	if (!audioFile.load(file)) {
		std::cout << "File " << file << " could not be loaded...\n";
		return 0;
	}
	else {
		std::cout << "File " << file << " loaded...\n";
		audioFile.printSummary();
		return 1;
	}

}

AudioBeat::AudioBeat(double sampleRate, double frameSize) {
	//initialise the gist values with new ones
	setAudioFrameSize(frameSize);
	setSamplingFrequency(sampleRate);
}

AudioBeat::AudioBeat() {
	//Gist initialised with default values
	//Frame size 512
	//Sampling rate 44100
}

AudioBeat::~AudioBeat() {

}
