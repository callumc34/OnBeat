#include <AudioBeatSDL/AudioBeatGame.h>

int FMOD_ERRCHECK(FMOD_RESULT r, const char * log, bool throwE) {
	//Return/Log error of FMOD
	if (r != FMOD_OK) {
		if (log != NULL) {
			std::fstream logFile;
			logFile.open(log, std::fstream::app);
			logFile << FMOD_ErrorString(r);
		}
		else {
			std::cout << FMOD_ErrorString(r) << std::endl;
		}

		if (throwE) {
			assert(r == FMOD_OK);
		}

		return 0;
	}
	return 1;
}

int AudioBeatGame::initAudioBeat(double frameSize, double sampleRate, const char* file) {
	//Process frames and setup the audiofile
	audioBeat.setAudioFrameSize(frameSize);
	audioBeat.setSamplingFrequency(sampleRate);
	std::cout << "Gist initialised...\nSample rate: "
		<< audioBeat.getSamplingFrequency() << "\nFrame size: "
		<< audioBeat.getAudioFrameSize() << std::endl;
	audioBeat.loadAudio(file);
	audioLocation = file;
	return 1;
}

int AudioBeatGame::initGainput() {
	//Define wanted key presses
	std::cout << "Gainput initialised...\n";
	return 1;
}

int AudioBeatGame::runGame() {
	initGainput();

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow(
		"Beat Hitter",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		1280,
		720,
		SDL_WINDOW_OPENGL
	);

	if (window == NULL) {
		std::cout << "Error initialising SDL Window";
		throw std::runtime_error("Error initialising SDL Window");
	}
	
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&sdlEvent) != 0) {
			if (sdlEvent.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	return 0;
}

AudioBeatGame::AudioBeatGame(double frameSize, double sampleRate, const char * file) {
	initAudioBeat(frameSize, sampleRate, file);
	AudioVector beats = audioBeat.processFrames();
	audioBeat.createBeatFile(
		beats, concatstr(getenv("APPDATA") != NULL ? getenv("APPDATA") : ".", "\\OnBeat\\beats\\"), "test.beat");
	
}

AudioBeatGame::AudioBeatGame() {

}

AudioBeatGame::~AudioBeatGame() {

}

int AudioPlayer::pauseAudio(bool pause) {
	//Set paused state of audio to passed var
	if (FMOD_ERRCHECK(
		channel->setPaused(pause)
	)) {
		system->update();
		return 0;
	}
	else {
		return 1;
	}
}

int AudioPlayer::playAudio() {
	//Reset audio if already playing
	//Plays audio from audioFile
	//Assigns channel handle
	if (playing) {
		if (!FMOD_ERRCHECK(channel->setPosition(0, FMOD_TIMEUNIT_MS))) {
			std::cout << "Error resetting position\n";
			return 0;
		}
		return 1;
	}


	if (!FMOD_ERRCHECK(
		system->playSound(
			sound,
			0,
			false,
			&channel
		))) {
		std::cout << "Error playing song " << audioFile << std::endl;
		return 0;
	}

	system->update();
	return 1;


}

int AudioPlayer::loadAudio(const char* audioLocation) {
	//Loads audio into FMOD player
	if (audioLocation == NULL && audioFile == NULL) {
		std::cout << "No audio file given\n";
		return 0;
	}

	audioFile = audioLocation;
	FMOD_ERRCHECK(system->createSound(
		audioFile,
		FMOD_DEFAULT,
		0,
		&sound
	), NULL, true);

	std::cout << audioFile << " loaded...\n";
	playing = false;

	loaded = true;

	return 1;

}

const char * AudioPlayer::getAudioFile() {
	return audioFile;
}

bool AudioPlayer::getPlaying() {
	if (!FMOD_ERRCHECK(channel->isPlaying(&playing))) {
		return playing;
	}
	else {
		return false;
	}
}

unsigned int AudioPlayer::getLength() {
	if (!FMOD_ERRCHECK(sound->getLength(&length, FMOD_TIMEUNIT_MS))) {
		return length;
	}
	else {
		return 0;
	}
}

bool AudioPlayer::getLoaded() {
	return loaded;
}

unsigned int AudioPlayer::getCurrentPos() {
	//Returns the current position in milliseconds
	if (!FMOD_ERRCHECK(channel->isPlaying(&playing))) {
		return 0;
	}
	else {
		if (!FMOD_ERRCHECK(channel->getPosition(&time, FMOD_TIMEUNIT_MS))) {
			return time;
		}
		else {
			return 0;
		}
	}
}

AudioPlayer::AudioPlayer(const char* audioLocation) {
	//Initialises the FMOD system
	audioFile = audioLocation;
	result = FMOD::System_Create(&system);
	if (!FMOD_ERRCHECK(result)) return;

	result = system->init(32, FMOD_INIT_NORMAL, 0);
	if (!FMOD_ERRCHECK(result)) return;

	if (audioLocation) {
		loadAudio(audioLocation);
	}
}

AudioPlayer::~AudioPlayer() {
	//Stop audio processing
	sound->release();
	delete[] audioFile;
	delete[] channel;
	delete[] system;	
}
