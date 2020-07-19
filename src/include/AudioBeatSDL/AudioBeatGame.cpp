#include <string>
#include <assert.h>
#include <FMOD/fmod_common.h>
#include <FMOD/fmod_errors.h>
#include <FMOD/fmod_output.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <RmlUi/Debugger.h>
#include <RmlUi/Core/Input.h>
#include <Shell.h>
#include <GL/glew.h>
#include <AudioBeatSDL/AudioBeatGame.h>

#ifdef RMLUI_PLATFORM_WIN32
#include <windows.h>
#endif

int FMOD_ERRCHECK(FMOD_RESULT r, const char * log, bool throwE) {
	//Return/Log error of FMOD
	if (r != FMOD_OK) {
		if (log != NULL) {
			std::fstream logFile;
			logFile.open(log, std::fstream::app);
			logFile << FMOD_ErrorString(r);
		}
		else {
			std::cerr << FMOD_ErrorString(r) << std::endl;
		}

		if (throwE) {
			assert(r == FMOD_OK);
		}

		return 0;
	}
	return 1;
}


#pragma region Timer


Timer::Timer()
{
	//Initialize the variables
	startTicks = 0;
	pausedTicks = 0;
	paused = false;
	started = false;
}

void Timer::start()
{
	//Start the timer
	started = true;

	//Unpause the timer
	paused = false;

	//Get the current clock time
	startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	//Stop the timer
	started = false;

	//Unpause the timer
	paused = false;
}

void Timer::pause()
{
	//If the timer is running and isn't already paused
	if ((started == true) && (paused == false))
	{
		//Pause the timer
		paused = true;

		//Calculate the paused ticks
		pausedTicks = SDL_GetTicks() - startTicks;
	}
}

void Timer::unpause()
{
	//If the timer is paused
	if (paused == true)
	{
		//Unpause the timer
		paused = false;

		//Reset the starting ticks
		startTicks = SDL_GetTicks() - pausedTicks;

		//Reset the paused ticks
		pausedTicks = 0;
	}
}

int Timer::get_ticks()
{
	//If the timer is running
	if (started == true)
	{
		//If the timer is paused
		if (paused == true)
		{
			//Return the number of ticks when the timer was paused
			return pausedTicks;
		}
		else
		{
			//Return the current time minus the start time
			return SDL_GetTicks() - startTicks;
		}
	}

	//If the timer isn't running
	return 0;
}

bool Timer::is_started()
{
	return started;
}

bool Timer::is_paused()
{
	return paused;
}


#pragma endregion Timer


#pragma region AudioBeatGame

int AudioBeatGame::initAudioBeat(double frameSize, double sampleRate) {
	//Process frames and setup the audiofile
	audioBeat.setAudioFrameSize(frameSize);
	audioBeat.setSamplingFrequency(sampleRate);
	std::cout << "Gist initialised...\nSample rate: "
		<< audioBeat.getSamplingFrequency() << "\nFrame size: "
		<< audioBeat.getAudioFrameSize() << std::endl;
	return 1;
}

int AudioBeatGame::initGainput() {
	//Define wanted key presses
	std::cout << "Gainput initialised...\n";
	return 1;
}

double AudioBeatGame::calculateBlitVelocity() {
	return (0.66 * height) / blitTiming;
}

int AudioBeatGame::createNewBeatScene() {
	audioBeat.loadAudio(audioLocation);
	AudioVector beats = audioBeat.processFrames();
	blitVelocity = calculateBlitVelocity();
	//Set up enki tasks here i guess

	return 1;
}

void AudioBeatGame::setWindowDimensions(SDL_DisplayMode screenDimensions) {
	width = screenDimensions.w;
	height = screenDimensions.h;
	monitorHz = (screenDimensions.refresh_rate > 0)
		? screenDimensions.refresh_rate : 60;
}

SDL_DisplayMode AudioBeatGame::getWindowDimensions() {
	SDL_DisplayMode dm;
	dm.w = width;
	dm.h = height;
	dm.refresh_rate = monitorHz;

	return dm;
}

SDL_DisplayMode AudioBeatGame::getFullScreenDimensions() {

	SDL_DisplayMode dm;

	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
		return dm;
	}

	return dm;
}

int AudioBeatGame::initSDL() {
	//Initialise SDL values and create a window

#ifdef RMLUI_PLATFORM_WIN32
	AllocConsole();
#endif
	
	std::cout << width << height;
	if (!width || !height) {
		setWindowDimensions(getFullScreenDimensions());
	}
	std::cout << width << height;
	//Set up SDL Window
	window = SDL_CreateWindow(
		"OnBeat",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
	);
	
	if (window == NULL) {
		std::cerr << "Error initialising SDL Window";
		throw std::runtime_error("Error initialising SDL Window");
		return 0;
	}

	//Set up SDL_gl 
	glcontext = SDL_GL_CreateContext(window);
	int oglIdx = -1;
	int nRD = SDL_GetNumRenderDrivers();
	for (int i = 0; i < nRD; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
		{
			if (!strcmp(info.name, "opengl"))
			{
				oglIdx = i;
			}
		}
	}	

	renderer = SDL_CreateRenderer(window, oglIdx, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //Set up OpenGL Renderer

	//Setup OpenGL
	GLenum err = glewInit();

	if (err != GLEW_OK)
		std::cerr << "Error intiialising glew...\n";
	

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glMatrixMode(GL_PROJECTION | GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(0, width, height, 0, 0, 1);

	Renderer.setRenderer(renderer); //Set up RmlUi renderer
	Renderer.setScreen(window);
	
	Rml::Core::SetFileInterface(&fileInterface);
	Rml::Core::SetRenderInterface(&Renderer);
	Rml::Core::SetSystemInterface(&SystemInterface);

	if (!Rml::Core::Initialise()) {
		return 1;
	}

	//Set up custom font
	Rml::Core::LoadFontFace(concatstr(exePath, "assets/Fonts/Walkway Expand Black.ttf"));
	Rml::Core::LoadFontFace(concatstr(exePath, "assets/Fonts/Walkway UltraExpand.ttf"));

	Context = Rml::Core::CreateContext("default",
		Rml::Core::Vector2i(width, height));

	Rml::Debugger::Initialise(Context);

	Context->EnableMouseCursor(true);
	Document = Context->LoadDocument(concatstr(exePath, "assets/rml/MainMenu/core.rml"));

	if (Document)
	{
		Document->Show();
		std::cout << "Document loaded\n";
	}
	else
	{
		std::cerr << "Could not load document\n";
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags)))
	{
		std::cerr << "Error initialising SDL_Image: " << IMG_GetError() << std::endl;
		return 0;
	}

	Document->AddEventListener("click", new RmlEventListener());

	return 1;
}

int AudioBeatGame::runGame() {
	int frame = 0;
	std::cout << "Running OnBeat...\n";

	while (!quit) {

		fps.start();

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		Context->Render();
		SDL_RenderPresent(renderer);


		while (SDL_PollEvent(&sdlEvent))
		{
			switch (sdlEvent.type)
			{
				case SDL_QUIT:
					quit = true;
					audioSys.releaseSound();
					break;

				case SDL_MOUSEMOTION:
					Context->ProcessMouseMove(sdlEvent.motion.x, sdlEvent.motion.y, SystemInterface.GetKeyModifiers());
					break;
				case SDL_MOUSEBUTTONDOWN:
					Context->ProcessMouseButtonDown(SystemInterface.TranslateMouseButton(sdlEvent.button.button), SystemInterface.GetKeyModifiers());
					break;

				case SDL_MOUSEBUTTONUP:
					Context->ProcessMouseButtonUp(SystemInterface.TranslateMouseButton(sdlEvent.button.button), SystemInterface.GetKeyModifiers());
					break;

				case SDL_MOUSEWHEEL:
					Context->ProcessMouseWheel(float(sdlEvent.wheel.y), SystemInterface.GetKeyModifiers());
					break;

				case SDL_KEYDOWN:
				{
					// Intercept F8 key stroke to toggle RmlUi's visual debugger tool
					if (sdlEvent.key.keysym.sym == SDLK_F8)
					{
						Rml::Debugger::SetVisible(!Rml::Debugger::IsVisible());
						break;
					}

					Context->ProcessKeyDown(SystemInterface.TranslateKey(sdlEvent.key.keysym.sym), SystemInterface.GetKeyModifiers());
					break;
				}

				default:
				{

					if (sdlEvent.user.code == UserEvent::Code::LaunchGame) {

						Document->Hide();
						createNewBeatScene();
					}
				}
			}
		
			Context->Update();

			
		} Context->Update();

		frame++; //Next frame
		
		if ((fps.get_ticks() < 1000 / frameRate)) {
				//Sleep the remaining frame time
				SDL_Delay((1000 / frameRate) - fps.get_ticks());
		}
	}


	return 0;
}


AudioBeatGame::AudioBeatGame(double frameSize, double sampleRate, const char * file) {

	SDL_Init(SDL_INIT_VIDEO);
	exePath = SDL_GetBasePath();
	if (!exePath) {
		exePath = "./";
	}
	if ((frameSize && sampleRate)) {
		initAudioBeat(frameSize, sampleRate);
	}
	audioLocation = file;
	if (audioLocation) {
		audioBeat.loadAudio(audioLocation);
	}

	initGainput();
}

AudioBeatGame::~AudioBeatGame() {
	Rml::Core::Shutdown();
	SDL_DestroyRenderer(renderer);
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);	
	SDL_Quit();
}

void AudioBeatGame::setBlitTiming(double timing) {
	blitTiming = timing;
}

double AudioBeatGame::getBlitTiming() {
	return blitTiming;
}

void AudioBeatGame::setBlitVelocity(double velocity) {
	blitVelocity = velocity;
}

double AudioBeatGame::getBlitVelocity() {
	return blitVelocity;
}
#pragma endregion AudioBeatGame

#pragma region AudioPlayer

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
			std::cerr << "Error resetting position\n";
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
		std::cerr << "Error playing song " << audioFile << std::endl;
		return 0;
	}

	system->update();
	return 1;


}

int AudioPlayer::loadAudio(const char* audioLocation) {
	//Loads audio into FMOD player
	if (audioLocation == NULL && audioFile == NULL) {
		std::cerr << "No audio file given\n";
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

int AudioPlayer::releaseSound() {
	sound->release();
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
	releaseSound();
	delete[] audioFile;
	delete[] channel;	
}

#pragma endregion AudioPlayer

#pragma region SDLScene

int SDLScene::renderBlit(const char* blitName, SDL_Surface * src, const SDL_Rect * srcrect,
	SDL_Rect * dstrect) {

	if (SDL_BlitSurface(src, srcrect, this, dstrect) != 0) {
		SDL_Log(SDL_GetError());
		return 0;
	}
	else {
		blits[blitName] = src;
		return 1;
	}
}

SDLScene::SDLScene() {
}

SDLScene::~SDLScene() {
	for (auto& blit : blits) {
		SDL_FreeSurface(blit.second);
		delete[] blit.second;
	}
}

#pragma endregion SDLScene