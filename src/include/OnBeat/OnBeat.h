#pragma once
#include <Hazel.h>
#include <AudioPlayer/AudioPlayer.h>
#include <OnBeat/Config/OnBeatConfig.h>
#include <string>
#include <filesystem>

class OnBeat : public Hazel::Application
{
	public:
		OnBeat();

		static float pxToGlF(float px);

		AudioPlayer AudioPlayer;
		Config::Settings Settings;

		~OnBeat();

	private:
		std::string iconPath = "logo\\logo-64.png";

		std::string cwd;

		void SetWindowIcon();
};