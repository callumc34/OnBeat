#include <OnBeat/OnBeat.h>

#include <Hazel/Core/EntryPoint.h>

#include <OnBeat/MusicLayer/MusicLayer.h>

OnBeat::OnBeat()
{
	PushLayer(new MusicLayer(this));
}

OnBeat::~OnBeat()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new OnBeat();
}