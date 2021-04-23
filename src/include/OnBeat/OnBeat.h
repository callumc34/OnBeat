#pragma once
#include <Hazel.h>
#include <AudioPlayer/AudioPlayer.h>
#include <string>

class OnBeat : public Hazel::Application
{
	public:
		OnBeat();

		static float pxToGlF(float px);

		AudioPlayer AudioPlayer;

		~OnBeat();

	private:
		std::string iconPath = "logo\\logo-64.png";

		void SetWindowIcon();
};