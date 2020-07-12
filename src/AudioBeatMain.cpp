#include <iostream>
#include <stdlib.h>
#include <AudioBeatGen.h>

int main(int argc, char *argv[]) {
	if (argc == 4) {
		double frameSize, sampleRate;
		frameSize = strtod(argv[1], NULL);
		sampleRate = strtod(argv[2], NULL);
		AudioBeat mainAudio (sampleRate, frameSize);
		std::cout << "Gainput initialised...\nGist initialised...\nSample rate: "
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