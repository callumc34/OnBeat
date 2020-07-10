#include <vector>
#include "AudioFile.h"
#include "gist.h"

struct AudioFlags {
	enum Value {
		SPECTRAL_DIFFERENCE = 0x01,
		ENERGY_DIFFERENCE 	= 0x02,
	};
};
 
class AudioBeat : public Gist<double> {

public:
	//Functions
	int loadAudio(const char* file);
	std::vector<std::vector<double>> processFrames(const char* logFile, AudioFlags::Value method);
	AudioBeat::AudioBeat(double sampleRate, double frameSize);
	AudioBeat::AudioBeat();
	AudioBeat::~AudioBeat();
	//Vars
	double getSampleRate();
	double getFrameSize();

private:
	//Functions
	//Vars
	AudioFile<double> audioFile;
	std::vector<std::vector<double>> frameDifferences;
};
