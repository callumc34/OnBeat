#pragma once
#include <OnBeat/Config/Config.h>
#include <OnBeat/App/LayerStack/LayerStack.h>
#include <OnBeat/App/MusicLayer/MusicLayer.h>
#include <OnBeat/Ui/MainMenu/MainMenu.h>
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

			void StartGame(const std::string& song);

			void RefreshSettings();
			int SetSettings(const Config::Settings& newS);

			//Get functions
			static App& Get() { return *instance; }
			GLFWwindow* GetNativeWindow() const { return NativeWindow; }
			LayerStack& GetLayerStack() { return *LayerStack; }
			AudioPlayer& GetAudioPlayer() { return AudioPlayer; }
			const Config::Settings& GetSettings() const { return Settings;  }

		private:
			static App* instance;

			void SetFullScreen(int monitor);
			void SetWindowIcon(const std::string& path);

			GLFWwindow* NativeWindow;
			LayerStack* LayerStack = new OnBeat::LayerStack();
			AudioPlayer AudioPlayer;
			Config::Settings Settings;

			//Layers
			MusicLayer* MusicLayer = nullptr;
			MainMenu* MainMenu = nullptr;
	};
}