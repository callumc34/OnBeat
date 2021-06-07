#include <OnBeat/App/App.h>
#include <OnBeat/Ui/MainMenu/MainMenu.h>
#include <Hazel/Core/EntryPoint.h>
#include <glfw/glfw3.h>
#include <stb_image/stb_image.h>
#include <imgui.h>

namespace OnBeat
{

	App* App::instance = nullptr;

	App::App()
		: Application("OnBeat")
	{
		Settings = Config::Settings::Create(OB_SETTINGS);
		instance = this;

		auto& window = Hazel::Application::Get().GetWindow();
		nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		SetWindowIcon("logo/logo-64.png");

		RefreshSettings();

		//Stop ImGui changing mouse cursor
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		PushLayer(new MainMenu(Settings.CurrentSkin.SkinDirectory + OB_MAIN_MENU, "Main Menu"));
	}

	void App::RefreshSettings()
	{
		if (!Config::validateSettings(Settings, false))
		{
			HZ_WARN("Settings refreshed with invalid settings. Falling back to default settings");
			Settings = Config::Settings::Create(OB_DEFAULT_SETTINGS);
		}
		SetFullScreen(Settings.Resolution.Fullscreen);
		AudioPlayer.setVolume(Settings.Volume);
	}

	void App::SetWindowIcon(const std::string& path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		HZ_ASSERT(channels == 4, "Icon must be RGBA!");

		GLFWimage images[1];
		images[0].width = width;
		images[0].height = height;
		images[0].pixels = data;
		glfwSetWindowIcon(nativeWindow, 1, images);
		stbi_image_free(data);
	}

	void App::SetFullScreen(int fs)
	{
		fs -= 1;

		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		if (fs > count)
		{
			//Error
			return;
		}		

		GLFWmonitor* monitor;
		int xOff, yOff;

		if (fs < 0)
		{
			monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			xOff = (mode->width - Settings.Resolution.DisplayWidth) / 2;
			yOff = (mode->height - Settings.Resolution.DisplayHeight) / 2;
			monitor = nullptr;
		}
		else
		{
			monitor = monitors[fs];
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			xOff = 0;
			yOff = 0;
		}


		// switch to full screen
		glfwSetWindowMonitor(nativeWindow, monitor, xOff, yOff, Settings.Resolution.DisplayWidth, Settings.Resolution.DisplayHeight, Settings.Resolution.FpsCap);
	}

	App::~App()
	{
	}
}

Hazel::Application* Hazel::CreateApplication()
{
#ifdef DIST
	FreeConsole();
#endif
	return new OnBeat::App();
}