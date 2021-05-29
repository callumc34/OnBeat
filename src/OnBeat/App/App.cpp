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
		: Application("OnBeat"), Settings("assets/user/UserConfig.json")
	{
		instance = this;

		auto& window = Hazel::Application::Get().GetWindow();
		nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());
		SetWindowIcon("logo/logo-64.png");

		cwd = std::filesystem::current_path().string();

		AudioPlayer.setVolume(Settings.getVolume());

		SetWindowState(Settings.getResolution().Fullscreen);

		//Stop ImGui changing mouse cursor
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

		PushLayer(new MainMenu(Settings.getCurrentSkin().SkinDirectory + "/menu/MainMenu/main.html", "Main Menu"));
	}

	void App::SetWindowState(int fs)
	{
		fs -= 1;
		if (fs >= 0)
		{
			SetFullScreen(fs);
		}
		else
		{
			glfwSetWindowSize(nativeWindow, Settings.getResolution().DisplayWidth, Settings.getResolution().DisplayHeight);
		}
	}

	void App::SetWindowIcon(const std::string& path)
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

	void App::SetFullScreen(int monitor)
	{
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
		glfwSetWindowMonitor(nativeWindow, glfwMonitor, 0, 0, mode->width, mode->height, Settings.getResolution().FpsCap);
	}

	App::~App()
	{
	}
}

Hazel::Application* Hazel::CreateApplication()
{
	return new OnBeat::App();
}