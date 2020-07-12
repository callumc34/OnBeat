#include <vector>
#include <AudioFile.h>
#include <Gist.h>
#include <portaudio.h>
#include <TaskScheduler.h>
#include <gainput/gainput.h>


//Custom flags
struct AudioFlags {
	enum Value {
		SPECTRAL_DIFFERENCE = 0x01,
		ENERGY_DIFFERENCE 	= 0x02,
	};
};

struct ParallelTaskSet : enki::ITaskSet {
	void ExecuteRange(enki::TaskSetPartition range_, uint32_t threadnum_) override {
		// do something here, can issue tasks with g_TS
	}
};

 
class AudioBeat : public Gist<double> {

public:
	//Functions
	int loadAudio(const char* file); //Load audio using AudioFile.h
	//Process each frame from the audio file using Gist and return a double vector of requested method
	std::vector<std::vector<double>> processFrames(const char* logFile = "./logs/frames.log",
		AudioFlags::Value method = AudioFlags::SPECTRAL_DIFFERENCE);
	AudioBeat::AudioBeat(double sampleRate, double frameSize); //Initialise with custom sample rate and frame size
	int AudioBeat::initGainput();
	AudioBeat::AudioBeat(); //Initialises gist with default values
	AudioBeat::~AudioBeat();
	//Vars

	//Gain input config
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
	int AudioBeat::initPA(); //Initialise the PA module;
	//Vars
	AudioFile<double> audioFile;
	std::vector<std::vector<double>> frameDifferences;
};
