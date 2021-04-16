#include <OnBeat/OnBeat.h>
#include <OnBeat/MusicLayer/MusicLayer.h>
#include <Hazel/Core/EntryPoint.h>

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