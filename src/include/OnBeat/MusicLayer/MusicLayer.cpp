#include <OnBeat/MusicLayer/MusicLayer.h>

MusicLayer::MusicLayer(OnBeat* app) 
	: Layer("MusicLayer")
{
	auto& window = Hazel::Application::Get().GetWindow();
	MainApp = app;
	
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

bool MusicLayer::OnWindowResize(Hazel::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

void MusicLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camWidth = 8.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	cameraController = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}