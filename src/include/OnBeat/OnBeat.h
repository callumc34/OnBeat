#pragma once
#include <Hazel.h>
#include <AudioPlayer/AudioPlayer.h>

class OnBeat : public Hazel::Application
{
	public:
		OnBeat();

		AudioPlayer AudioPlayer;

		~OnBeat();
};