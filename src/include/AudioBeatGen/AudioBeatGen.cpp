#include <assert.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <math.h>
#include <AudioBeatGen/AudioBeatGen.h>
#include <sys/stat.h>

bool fileExists(const char* file) {
	struct stat buffer;
	return (stat(file, &buffer) == 0);
}

const char* concatstr(const char* s1, const char* s2) {
	return (std::string(s1) + std::string(s2)).c_str();
}

bool validateAudioVector(AudioVector av, double frameSize, double sampleRate, double length) {
	if (round((av[0].size() / length)) != round(sampleRate / frameSize)) {
		std::cout << "Sample count differs from sample rate and frame size\n";
		return false;
	}


	if (av.size() > 1) { //Validate all channels are equal in size
		for (int i = 1; i < av.size() - 1; i++)
			if (av[i].size() != av[-1].size()) {
				std::cout << "Channels differ in size\n";
				return false;
			}
	}

	return true;
}

AudioVector AudioBeat::parseBeatFile(const char* beatFile) {
	std::ifstream beatStream(beatFile, std::fstream::in);
	std::string line;
	if (!beatStream.is_open()) {
		std::cout << "Error opening file " << beatFile << std::endl;
		return AudioVector{}; //Return empty audio vector
	}

	AudioVector beats;
	std::string buffer, sr;
	bool beginBeat = false;
	int curChannel = 0;

	int lineNum = 0;

	while (std::getline(beatStream, line)) {
		if (line.substr(0, 10) == "BEGIN BEAT") beginBeat = true; //Do not parse lines until beat is defined
		if (line.substr(0, 8) == "END BEAT") beginBeat = false; //End parsing of beat
		if (beginBeat) {
			if (line.substr(0, 11) == "NEW CHANNEL") {
				beats.push_back(std::vector<double>{});
				curChannel += 1;
			}
			else {
				std::size_t end = line.find(";");
				if (end == std::string::npos) {
					std::cout << "No end to line - Error parsing line " << lineNum << std::endl;
					return AudioVector{};
				}

				beats[curChannel].push_back(strtod(line.substr(0, end).c_str(), NULL));
			}
		}
		else if (line.substr(0, 6) == "DEFINE") {
			std::size_t assignLocation = line.find("=");
			std::size_t endLocation = line.find(";");
			if (assignLocation == std::string::npos || endLocation == std::string::npos) {
				std::cout << "No end to line - Error parsing line " << lineNum << std::endl;
				return AudioVector{};
			}
			std::string varName = line.substr(7, assignLocation - 7);
			std::string var = line.substr(assignLocation + 1, endLocation - assignLocation - 1);
			if (varName == "BUFFER") {
				buffer = var;
			}
			else if (varName == "SAMPLE_RATE") {
				sr = var;
			}
			else {
				std::cout << "Error parsing var in " << beatFile << ": UNKNOWN VAR " << varName << "\nErr line: " << lineNum;
				return AudioVector{};
			}
		}
		else {}

		lineNum += 1;

	}

	

	return beats;
}

int AudioBeat::createBeatFile(AudioVector beats, const char* outputDir, const char* outputName) {
	if (!std::experimental::filesystem::create_directories(outputDir)) {
		std::cout << "Error making dir " << outputDir << "\nDir may exist\n";
	}

	const char* outputFile = concatstr(outputDir, outputName);

	std::ofstream outputStream(outputFile);

	if (!outputStream.is_open()) {
		std::cout << "Error opening file " << outputFile << std::endl;
		return 0;
	}
	outputStream << "DEFINE BUFFER=" << getAudioFrameSize() << ";\nDEFINE SAMPLE_RATE=" << getSamplingFrequency() << ";\n";
	outputStream << "BEGIN BEAT;\n";

	for (int c = 0; c < beats.size(); c++) {
		outputStream << "NEW CHANNEL;\n";
		for (int i = 0; i < beats[c].size(); i++) {
			outputStream << beats[c][i] << ";\n";
		}
	}

	outputStream << "END BEAT;";

	std::cout << "File written to " << concatstr(outputDir, outputName) << std::endl;

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

	assert(validateAudioVector(beats, getAudioFrameSize(),
		getSamplingFrequency(), audioFile.getLengthInSeconds()));
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
