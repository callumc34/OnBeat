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

int FMOD_ERRCHECK(FMOD_RESULT r, const char* log = "./logs/FMOD.log", bool throwE = false);

std::string calculateOffsetHeight(double velocity, double timeOffset);

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
class SDLScene { //Simple scene implementation
public:
	//Functions
	//Redefinition if BlitSurface but also appends to blits vector
	SDL_Surface* getScene() {
		return scene;
	}
	void setRunning(bool run);
	bool isRunning();
	virtual void onFrame() {}; //Render on every frame
	virtual void startScene() {};
	SDLScene(int width, int height);
	~SDLScene();
	//Vars

protected:
	Uint32 eventType = SDL_RegisterEvents(1);
	SDL_Surface* scene;

private:
	//Functions
	//Vars
	bool running = false;
};

class RhythmScene : public SDLScene {
public:
	//Functions
	RhythmScene(int width, int height, double blitTiming, AudioVector newBeats);
	void onFrame() override;
	void startScene() override;
	int finishedRunning();
	
	//Vars
	Timer beatTimer;

private:
	//Functions
	void sendBlitEvent(int timeStamp);
	//Vars
	AudioVector beats;
	double blitOn;
	int blitOnMS;
	int previousTick = 0;
	int currentBeat = 0;
};


#pragma endregion SDLScene

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
	int loadNewDocument(const char * documentPath);
	int loadNewTheme(const char* themeDir);
	int initAudioBeat(double frameSize, double sampleRate);
	int runGame();
	int createNewBeatScene();
	double calculateBlitVelocity();
	double imgHeight;
	void updateBeats();
	AudioBeatGame(double frameSize = NULL, double sampleRate = NULL, const char* file = NULL);
	~AudioBeatGame();

	//Fetch/Get functions
	//Blit timing
	void setBlitTiming(double timing);
	double getBlitTiming();
	//Threshold
	double getThreshold(int channel);
	void setThreshold(int channel, int threshold);
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
	int addBeatBlit(SDLScene * scene, int channel, double beatStrength, double timeOffset);
	//Vars
	std::unordered_map<const char*, SDLScene*> scenes;
	double blitTiming = 1;
	double blitVelocity;
	double monitorHz;
	double frameRate;
	double beatThreshold1 = 1000;
	double beatThreshold2 = 1000;
	int lastUpdate = 0;
	int width = NULL;
	int height = NULL;
	const char* audioLocation;
	const char* exePath = std::filesystem::current_path().string().c_str();
	const char* themeLocation;
	bool quit = false;
	AudioBeat audioBeat;
	RhythmScene* rhythmSurface;

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
};

#pragma endregion AudioBeatGame

#endif