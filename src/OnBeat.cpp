#include <OnBeat/Ui/OnBeatMenu.h>
#include <OnBeat/OnBeat.h>
#include <Hazel/Core/EntryPoint.h>

#include <stb_image/stb_image.h>

OnBeat::OnBeat()
	: Application("OnBeat"), Settings("assets/user/UserConfig.json")
{
	auto& window = Hazel::Application::Get().GetWindow();
	nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
	SetWindowIcon(iconPath);

	cwd = std::filesystem::current_path().string();

	AudioPlayer.setVolume(Settings.Volume);

	int fs = Settings.Fullscreen - 1;

	if (fs > 0)
	{
		SetFullScreen(fs);
	}
	else
	{
		glfwSetWindowSize(nativeWindow, Settings.DisplayWidth, Settings.DisplayHeight);
	}

	PushLayer(new OnBeatMenu(Settings.CurrentSkinPath + "/menu/MainMenu/main.html"));
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

void OnBeat::SetWindowIcon(std::string path)
{

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