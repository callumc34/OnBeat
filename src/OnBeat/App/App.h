#pragma once
#include <OnBeat/Config/Config.h>
#include <OnBeat/Util/AudioPlayer/AudioPlayer.h>
#include <Hazel/Core/Application.h>
#include <glfw/glfw3.h>

namespace OnBeat
{
	class App : public Hazel::Application
	{

		public:
			App();
			~App();

			AudioPlayer AudioPlayer;
			Config::Settings Settings;

			void SetWindowIcon(const std::string& path);
			//Window resize funcs
			void SetFullScreen(int monitor);
			void SetWindowed(int x, int y, int width, int height);

			GLFWwindow* GetNativeWindow() const { return nativeWindow; }

		private:
			GLFWwindow* nativeWindow;
			std::string cwd;
	};
}