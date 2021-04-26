#pragma once
#include <Hazel.h>
#include <AudioPlayer/AudioPlayer.h>
#include <OnBeat/Config/OnBeatConfig.h>
#include <string>
#include <filesystem>
#include <GLFW/glfw3.h>

class OnBeat : public Hazel::Application
{
	public:
		OnBeat();

		static float pxToGlF(float px);

		AudioPlayer AudioPlayer;
		Config::Settings Settings;

		void SetFullScreen(int monitor);
		void SetWindowed(int x, int y, int width, int height);

		GLFWwindow* getNativeWindow() const { return nativeWindow; };

		~OnBeat();

	private:
		std::string iconPath = "logo\\logo-64.png";

		std::string cwd;

		GLFWwindow* nativeWindow;

		void SetWindowIcon();
};