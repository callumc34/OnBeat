#pragma once
#include <OnBeat/Config/Config.h>
#include <OnBeat/Util/AudioPlayer/AudioPlayer.h>
#include <Hazel/Core/Application.h>

typedef struct GLFWwindow GLFWwindow;

namespace OnBeat
{
	class App : public Hazel::Application
	{

		public:
			App();
			~App();

			static App& Get() { return *instance; }

			AudioPlayer AudioPlayer;
			Config::Settings Settings;

			void SetWindowIcon(const std::string& path);
			//Window resize funcs
			void SetFullScreen(int monitor);
			void SetWindowed(int x, int y, int width, int height);

			GLFWwindow* GetNativeWindow() const { return nativeWindow; }

		private:
			static App* instance;

			GLFWwindow* nativeWindow;
			std::string cwd;
	};
}