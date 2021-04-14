#include <OnBeat/OnBeat.h>

#include <Hazel/Core/EntryPoint.h>

#include <OnBeat/OnBeatGame.h>

OnBeat::OnBeat()
{
	PushLayer(new OnBeatGame());
}

OnBeat::~OnBeat()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new OnBeat();
}