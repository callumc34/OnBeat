#include <vector>
#include <Gist/AudioFile.h>
#include <Gist/Gist.h>
#include <TaskScheduler/TaskScheduler.h>
#include <gainput/gainput.h>

#ifndef AUDIOBEAT
#define AUDIOBEAT

typedef std::vector<std::vector<double>> AudioVector; //Simplify 2d vector of doubles

//Custom flags
struct AudioBeatFlags {
	enum Value {
		SPECTRAL_DIFFERENCE = 0x01,
		ENERGY_DIFFERENCE 	= 0x02,
	};
};
 
class AudioBeat : public Gist<double> {

public:
	//Functions
	int loadAudio(const char* file); //Load audio using AudioFile.h
	int createBeatFile(AudioVector beats, const char * outputDir);
	AudioVector cleanUpBeats(AudioVector beats, const char* logFile = "./logs/frames.log");
	//Process each frame from the audio file using Gist and return a double vector of requested method
	AudioVector processFrames(const char* logFile = "./logs/frames.log",
		AudioBeatFlags::Value method = AudioBeatFlags::SPECTRAL_DIFFERENCE);
	AudioBeat(double sampleRate, double frameSize); //Initialise with custom sample rate and frame size
	AudioBeat(); //Initialises gist with default values
	~AudioBeat();
	//Vars
	AudioFile<double> audioFile;


private:
	//Functions
	//Vars
};

bool fileExists(const char* file);

#endif