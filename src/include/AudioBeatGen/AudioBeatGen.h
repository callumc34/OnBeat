#include <vector>
#include <Gist/AudioFile.h>
#include <Gist/Gist.h>

typedef std::vector<std::vector<double>> AudioVector; //Simplify 2d vector of doubles

bool fileExists(const char* file);

const char * concatstr(const char * s1, const char * s2);

bool validateAudioVector(AudioVector av, double frameSize, double sampleRate, double length);

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
	int loadAudio(const char* file); //Use audioFile.h to load the audio file
	AudioVector parseBeatFile(const char * beatFile); //Parses the given beat file
	int createBeatFile(AudioVector beats, const char * outputDir, const char* outputName); //Creates a beat file from the given AudioVector
	AudioVector cleanUpBeats(AudioVector beats, const char* logFile = "./logs/frames.log"); //Removes beats tto close to each other
	//Process each frame from the audio file using Gist and return a double vector of requested method
	AudioVector processFrames(const char* logFile = "./logs/frames.log",
		AudioBeatFlags::Value method = AudioBeatFlags::SPECTRAL_DIFFERENCE); //Find beats above threshold
	AudioBeat(double sampleRate, double frameSize); //Initialise with custom sample rate and frame size
	AudioBeat(); //Initialises gist with default values
	~AudioBeat();
	//Vars
	AudioFile<double> audioFile;


private:
	//Functions
	//Vars
};