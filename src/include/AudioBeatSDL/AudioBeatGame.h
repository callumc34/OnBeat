#ifndef AUDIOBEATGAME_H
#define AUDIOBEATGAME_H

#include <string>
#include <thread>
#include <filesystem>
#include <TaskScheduler/TaskScheduler.h>
#include <FMOD/fmod.hpp>
#include <gainput/gainput.h>
#include <ShellFileInterface.h>
#include <RmlUi/SDL2Integration/RenderInterfaceSDL2.h>
#include <RmlUi/SDL2Integration/SystemInterfaceSDL2.h>
#include <AudioBeatGen/AudioBeatGen.h>
#include <AudioBeatSDL/RmlEventListener.hpp>

const std::string APPDATA = getenv("APPDATA") != NULL ? getenv("APPDATA") : ".";

const uint32_t NUMEXTERNALTHREADS = 5;

int FMOD_ERRCHECK(FMOD_RESULT r, const char* log = "./logs/FMOD.log", bool throwE = false);

#pragma region Timer
class Timer //Simple timer used to regulare frame rate
{

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();
	void stop();
	void pause();
	void unpause();

	//Gets the timer's time
	int get_ticks();

	//Checks the status of the timer
	bool is_started();
	bool is_paused();

private:
	//The clock time when the timer started
	int startTicks;

	//The ticks stored when the timer was paused
	int pausedTicks;

	//The timer status
	bool paused;
	bool started;

};
#pragma endregion Timer

#pragma region SDLScene
class SDLScene : public SDL_Surface { //Simple scene implementation
public:
	//Functions
	//Redefinition if BlitSurface but also appends to blits vector
	int renderBlit(const char* blitName, SDL_Surface* src, const SDL_Rect* srcrect, SDL_Rect* dstrect);
	virtual void onFrame() = 0; //Render on every frame
	bool isActive();
	void setActive(bool display);
	SDLScene();
	~SDLScene();
	//Vars
private:
	//Functions
	//Vars
	bool active;
	std::unordered_map<const char*, SDL_Surface*> blits;
};

class RhythmScene : SDLScene {
public:
	//Functions
	void onFrame() override;
	//Vars
	Timer beatTimer;

private:
	//Functions
	int finishedRunning();

	//Vars
	AudioVector beats;
	double blitOn;
	Uint32 eventType = SDL_RegisterEvents(1);
};


#pragma endregion SDLScene

//Probably delete
#pragma region ParallelBlitCreater
struct ParallelBlitCreater : enki::ITaskSet {
	const double blitOn;
	const AudioVector beats;
	Timer blitTimer;
	bool running = false;
	bool finished = false;
	Uint32 eventType = SDL_RegisterEvents(1);

	ParallelBlitCreater(double blitOn, AudioVector beats) : blitOn(blitOn), beats(beats) {

	}

	void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override {
		SDL_Event blitEvent;
		blitEvent.type = eventType;
		blitEvent.user.code = NULL;
		blitEvent.user.data1 = NULL;
		blitEvent.user.data2 = NULL;
		blitTimer.start();

		for (int i = 0; i < beats[0].size(); i++) {
			while (blitTimer.is_paused()) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			blitEvent.user.code = UserEvent::Code::CREATE_BLIT;
			blitEvent.user.data1 = malloc(sizeof beats[0][i]); 
			if (blitEvent.user.data1)
				*(double *)blitEvent.user.data1 = beats[0][i];

			blitEvent.user.data2 = malloc(sizeof beats[1][i]); 
			if (blitEvent.user.data2)
				*(double *)blitEvent.user.data2 = beats[1][i];

			if ((blitOn * (i + 1)) / 1000 >= blitTimer.get_ticks()) { //If beat is due
				SDL_PushEvent(&blitEvent);
				if (beats[0][i] > 10) {
					std::cout << "Beat sent: " << i + 1;
				}
			}
			else { //Sleep for thte remaining time
				//std::chrono::duration<double std::chrono::milliseconds> sleepTime = (blitOn * (i + 1)) - blitTimer.get_ticks();
				std::this_thread::sleep_for(
					std::chrono::duration<double>((blitOn * (i + 1)) - blitTimer.get_ticks()));
			}
		}

	}
};
#pragma endregion ParallelBlitCreater

#pragma region AudioPlayer
class AudioPlayer { //Plays audio using the FMOD libraries

public:
	//Functions
	int pauseAudio(bool pause);
	int playAudio();
	int loadAudio(const char* audioLocation = NULL);
	int releaseSound();

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
#pragma endregion AudioPlayer


#pragma region AudioBeatGame
class AudioBeatGame { //Base class for the game

public:
	//Functions
	int initSDL();
	int initAudioBeat(double frameSize, double sampleRate);
	int runGame();
	int createNewBeatScene();
	AudioBeatGame(double frameSize = NULL, double sampleRate = NULL, const char* file = NULL);
	~AudioBeatGame();

	//Fetch/Get functions
	//Blit timing
	void setBlitTiming(double timing);
	double getBlitTiming();
	//Blit velocity
	void setBlitVelocity(double velocity);
	double getBlitVelocity();
	//Screen vals
	SDL_DisplayMode getFullScreenDimensions();
	void setWindowDimensions(SDL_DisplayMode screenDimensions);
	SDL_DisplayMode getWindowDimensions();

	//Vars
	Timer fps;

	//Gainput config
	gainput::InputManager manager; //Gainput manager
	const gainput::DeviceId keyboardId = manager.CreateDevice<gainput::InputDeviceKeyboard>();
	const gainput::DeviceId mouseId = manager.CreateDevice<gainput::InputDeviceMouse>();
	const gainput::DeviceId padId = manager.CreateDevice<gainput::InputDevicePad>();
	const gainput::DeviceId touchId = manager.CreateDevice<gainput::InputDeviceTouch>();
	gainput::InputMap map{ manager };
	

private:
	//Functions
	int initGainput();
	int initEnki();
	double calculateBlitVelocity();
	//Vars
	std::unordered_map<const char*, SDLScene> scenes;
	double blitTiming = 2;
	double blitVelocity;
	double monitorHz;
	double frameRate;
	int width;
	int height;
	const char* audioLocation;
	const char* exePath = std::filesystem::current_path().string().c_str();
	bool quit = false;
	AudioBeat audioBeat;

	//SDL Vars
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event sdlEvent;
	SDL_Surface* windowSurface;
	SDL_GLContext glcontext;
	RmlUiSDL2SystemInterface SystemInterface;
	RmlUiSDL2Renderer Renderer;
	ShellFileInterface fileInterface = ShellFileInterface(exePath);
	Rml::Core::ElementDocument* Document;
	Rml::Core::Context* Context;

	//FMOD Vars
	AudioPlayer audioSys;

	//Enki Task Scheduler config
	enki::TaskScheduler enkiTS;
	ParallelBlitCreater* blitScheduler;
};

#pragma endregion AudioBeatGame

#endif