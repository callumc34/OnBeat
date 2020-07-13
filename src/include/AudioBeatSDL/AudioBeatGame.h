#include <SDL2/sdl.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_common.h>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_output.h>
#include <AudioBeatGen/AudioBeatGen.h>

int FMOD_ERRCHECK(FMOD_RESULT r, const char* log = "./logs/FMOD.log", bool throwE = false);

struct ParallelTaskSet : enki::ITaskSet {
	void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override {
		// do something here, can issue tasks with g_TS
	}
};

class AudioPlayer {

public:
	//Functions
	int pauseAudio(bool pause);
	int playAudio();
	int loadAudio(const char* audioLocation = NULL);

	const char* getAudioFile();
	unsigned int getCurrentPos();
	bool getPlaying();
	unsigned int getLength();
	bool getLoaded();
	AudioPlayer(const char * audioFile = NULL);
	~AudioPlayer();
	//Vars
	FMOD_RESULT result;
private:
	//Functions
	//Vars
	FMOD::System *system;
	FMOD::Sound *sound;
	FMOD::Channel *channel = 0;
	int channelID = 0;
	const char* audioFile;
	unsigned int time = 0;
	unsigned int length = 0;
	bool playing = false;
	bool loaded = false;
};

class AudioBeatGame {

public:
	//Functions
	int initAudioBeat(double frameSize, double sampleRate, const char * file);
	int runGame();
	AudioBeatGame(double frameSize, double sampleRate, const char* file);
	AudioBeatGame();
	~AudioBeatGame();
	//Vars
	SDL_Window* window;
	SDL_Event sdlEvent;
	AudioBeat audioBeat;

	//Gainput config
	gainput::InputManager manager; //Gainput manager
	const gainput::DeviceId keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
	const gainput::DeviceId mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
	const gainput::DeviceId padId = manager.CreateDevice<gainput::InputDevicePad>();
	const gainput::DeviceId touchId = manager.CreateDevice<gainput::InputDeviceTouch>();
	gainput::InputMap map{ manager };

	//Enki Task Scheduler config
	enki::TaskScheduler g_TS;
	ParallelTaskSet task;
	

private:
	//Functions
	int initGainput();
	//Vars
	int width;
	int height;
	int framesPerBuffer = 512;
	const char* audioLocation;

	//FMOD init
	AudioPlayer audioSys;
};