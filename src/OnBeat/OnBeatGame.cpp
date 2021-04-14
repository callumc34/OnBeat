#include <OnBeat/OnBeatGame.h>

OnBeatGame::OnBeatGame() 
	: Layer("OnBeatGame"), cameraController(1280.0f / 720.0f)
{
	auto& window = Hazel::Application::Get().GetWindow();
}

void OnBeatGame::OnAttach()
{

}

void OnBeatGame::OnDetach()
{

}

void OnBeatGame::OnUpdate(Hazel::Timestep ts)
{

}

void OnBeatGame::OnEvent(Hazel::Event& e)
{

}

void OnBeatGame::OnImGuiRender()
{

}