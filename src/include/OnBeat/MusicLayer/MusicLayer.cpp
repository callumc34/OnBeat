#include <OnBeat/MusicLayer/MusicLayer.h>

MusicLayer::MusicLayer(Hazel::Application* app) 
	: Layer("MusicLayer"), cameraController(1280.0f / 720.0f)
{
	auto& window = Hazel::Application::Get().GetWindow();
	app = app;
}

void MusicLayer::OnAttach()
{

}

void MusicLayer::OnDetach()
{

}

void MusicLayer::OnUpdate(Hazel::Timestep ts)
{

}

void MusicLayer::OnEvent(Hazel::Event& e)
{

}

void MusicLayer::OnImGuiRender()
{

}