#include <vector>
#include <AudioFile.h>
#include <Gist.h>
#include <portaudio.h>


//Custom flags
struct AudioFlags {
	enum Value {
		SPECTRAL_DIFFERENCE = 0x01,
		ENERGY_DIFFERENCE 	= 0x02,
	};
};
 
class AudioBeat : public Gist<double> {

public:
	//Functions
	int loadAudio(const char* file); //Load audio using AudioFile.h
	//Process each frame from the audio file using Gist and return a double vector of requested method
	std::vector<std::vector<double>> processFrames(const char* logFile, AudioFlags::Value method); 
	AudioBeat::AudioBeat(double sampleRate, double frameSize); //Initialise with custom sample rate and frame size
	AudioBeat::AudioBeat(); //Initialises gist with default values
	AudioBeat::~AudioBeat();
	//Vars

private:
	//Functions
	int AudioBeat::initPA(); //Initialise the PA module;
	//Vars
	AudioFile<double> audioFile;
	std::vector<std::vector<double>> frameDifferences;
};
