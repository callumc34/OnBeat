#include <OnBeat/OnBeat.h>
#include <OnBeat/MusicLayer/MusicLayer.h>
#include <Hazel/Core/EntryPoint.h>

#include <stb_image/stb_image.h>

OnBeat::OnBeat()
	: Application("OnBeat")
{
	auto& window = Hazel::Application::Get().GetWindow();
	nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
	SetWindowIcon();

	cwd = std::filesystem::current_path().string();
	Settings = Config::Settings((cwd + "/assets/user/UserConfig.json").c_str());

	int fs = Settings.Fullscreen - 1;

	if (fs > 0)
	{
		SetFullScreen(fs);
	}
	else
	{
		glfwSetWindowSize(nativeWindow, Settings.DisplayWidth, Settings.DisplayHeight);
	}

	//Temp test music
	OnSetGen gen(0, 1, 8, 8);
	auto all = gen.processFile("testing/testMusic/test.wav");
	auto beats = gen.findBeats(all);
	gen.createBeatFile(beats, "testing/graphing/testBeats.log");
	PushLayer(new MusicLayer(this, beats,
		900.0f, 44100, 512));
}

float OnBeat::pxToGlF(float px)
{
	return px / 100;
}

void OnBeat::SetWindowed(int x, int y, int width, int height)
{
	glfwSetWindowMonitor(nativeWindow, nullptr, x, y, width, height, Settings.FpsCap);
}

void OnBeat::SetFullScreen(int monitor)
{
    int _wndPos [2];
    int _wndSize[2];
    // backup window position and window size
    glfwGetWindowPos(nativeWindow, &_wndPos[0], &_wndPos[1]);
    glfwGetWindowSize(nativeWindow, &_wndSize[0], &_wndSize[1]);

    // get resolution of monitor
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    if (monitor > count)
    {
        //Error
        return;
    }

    GLFWmonitor* glfwMonitor = monitors[monitor];
        
    const GLFWvidmode* mode = glfwGetVideoMode(glfwMonitor);

    // switch to full screen
    glfwSetWindowMonitor(nativeWindow, glfwMonitor, 0, 0, mode->width, mode->height, Settings.FpsCap);
    
}

void OnBeat::SetWindowIcon()
{
	
	std::string path = iconPath;

	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	HZ_ASSERT(channels == 4, "Icon must be RGBA!");

	GLFWimage images[1];
	images[0].width = width;
	images[0].height = height;
	images[0].pixels = data;
	glfwSetWindowIcon(nativeWindow, 1, images);
	stbi_image_free(data);
}

OnBeat::~OnBeat()
{
	
}

Hazel::Application* Hazel::CreateApplication()
{
	return new OnBeat();
}