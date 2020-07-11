#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <AudioBeatMain.h>

std::vector<std::vector<double>> AudioBeat::processFrames(const char* logFile = "./logs/frames.log",
	AudioFlags::Value method = AudioFlags::SPECTRAL_DIFFERENCE) {

	if (audioFile.getFileFormat() == AudioFileFormat::NotLoaded) {
		throw std::runtime_error("Audio file not loaded");
	}
	std::ofstream logStream (logFile);
	if (!logStream.is_open()) {
		throw std::runtime_error("Error opening file log file");
	} else {

	}

	frameDifferences.clear();

	std::vector<double> frame;

	for (int c = 0; c < audioFile.samples.size(); c++) {
		std::cout << "Processing channel: " << c + 1 << std::endl;
		// << "\nChannel size: " << audioFile.samples[c].size()
		// << "\nSamples to process: " <<
		//  audioFile.samples[c].size() / getAudioFrameSize() << std::endl;

		std::vector<double> channel;
		frameDifferences.push_back(channel);

		for (int i = getAudioFrameSize(); i < audioFile.samples[c].size(); i += getAudioFrameSize()) {
			for (int buf = i-getAudioFrameSize(); buf < i; buf++) {

				frame.push_back(audioFile.samples[c][buf]);

			}

			// std::cout << "Audio frame [" << i/getAudioFrameSize() << "] of [" << audioFile.samples[c].size()/getAudioFrameSize() << "]\n"; 
			logStream << "Audio frame [" << i/getAudioFrameSize() << "] of [" << audioFile.samples[c].size()/getAudioFrameSize() << "]\n";
			processAudioFrame(frame);

			double eD = energyDifference();
			double sD = spectralDifference();

			logStream << "Energy difference: " << eD
			 << "\nSpectral difference: " << sD << std::endl;

			frameDifferences[c].push_back(
				(method == AudioFlags::SPECTRAL_DIFFERENCE)
				 ? sD : eD);

			// std::cout << "Energy difference: " << eD
			//  << "\nSpectral difference: " << sD << std::endl;

			frame.clear();
		}

		channel.clear();

	}

	logStream.close();

	//Vectors must be of equal size for time to correspond to difference
	assert(frameDifferences[0].size() ==
	 audioFile.samples[0].size() / getAudioFrameSize());

	if (frameDifferences.size() > 1) {
		for (int i = 1; i < frameDifferences.size() -1; i++)
		 assert(frameDifferences[i].size() == frameDifferences[-1].size());
	}

	std::cout << "Finished processing frames\n";

	return frameDifferences;
}

int AudioBeat::loadAudio(const char* file) {
	//Load audio using AudioFile.h to process the frames
	std::cout << "Loading file: " << file << "...\n";
	if (!audioFile.load(file)) {
		std::cout << "File " << file << " could not be loaded...\n";
		return 0;
	} else {
		std::cout << "File " << file << " loaded...\n";
		audioFile.printSummary();
		return 1;
	}

}

int AudioBeat::initPA() {
	//Initialise the PA module
	PaError err = Pa_Initialize();
	if (err != paNoError) {
		throw std::runtime_error(Pa_GetErrorText(err));
		return false;
	}
	else {
		std::cout << "PA Initialised...\n";
		return true;
	}
}

AudioBeat::AudioBeat(double sampleRate, double frameSize) {
	//initialise the gist values with new ones
 	setAudioFrameSize(frameSize);
 	setSamplingFrequency(sampleRate);
	initPA();
}

AudioBeat::AudioBeat() {
	//Gist initialised with default values
	//Frame size 512
	//Sampling rate 44100
	initPA();
}

AudioBeat::~AudioBeat() {

}

int main(int argc, char *argv[]) {
	if (argc == 4) {
		double frameSize, sampleRate;
		frameSize = strtod(argv[1], NULL);
		sampleRate = strtod(argv[2], NULL);
		AudioBeat mainAudio (sampleRate, frameSize);
		std::cout << "Gist initialised...\nSample rate: "
		 << mainAudio.getSamplingFrequency() << "\nFrame size: "
		  << mainAudio.getAudioFrameSize() << std::endl;
		mainAudio.loadAudio(argv[3]);
		mainAudio.processFrames();
	} else {
		std::cout << 
		"USAGE: AudioBeatMain [frame size] [sample rate] [file]\n";
		return 0;
	}
	return 1;
}