#include <stdlib.h>
#include <iostream>
#include <AudioBeatSDL/AudioBeatGame.h>

const char* APPDATA = getenv("APPDATA") != NULL ? getenv("APPDATA") : "./beats/";

int main(int argc, char *argv[]) {
	if (argc == 4) {
		double frameSize, sampleRate;
		try {
			frameSize = strtod(argv[1], NULL);
			sampleRate = strtod(argv[2], NULL);
		}
		catch(const std::exception& e) {
			std::cout << "Error converting args to double" << e.what() << std::endl;
			return 0;
		}
		frameSize = strtod(argv[1], NULL);
		sampleRate = strtod(argv[2], NULL);
		AudioBeatGame game(frameSize, sampleRate, argv[3]);
	} else {
		std::cout << 
		"USAGE: AudioBeatMain [frame size] [sample rate] [file]\n";
		return 0;
	}
	return 1;
}