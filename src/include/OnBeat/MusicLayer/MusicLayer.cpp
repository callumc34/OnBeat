#include <OnBeat/MusicLayer/MusicLayer.h>

MusicLayer::MusicLayer(OnBeat* app) 
	: Layer("MusicLayer")
{
	MainApp = app;
	auto& window = app->GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());

	//Initialise shaders
	TextureShader = ShaderLibrary.Load("assets/shaders/Texture.glsl");
	FlatColorShader = ShaderLibrary.Load("assets/shaders/FlatColor.glsl");
}

void MusicLayer::CreateBeatArea()
{

	Hazel::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f });
	//Lines
	Hazel::Renderer2D::DrawQuad({ -5.0f, 0.0f }, { 0.1f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ -2.5f, 0.0f }, { 0.1f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 0.1f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 2.5f, 0.0f }, { 0.1f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 5.0f, 0.0f }, { 0.1f, 20.0f }, { 1.0f, 1.0f, 1.0f, 1.0f });
	//Beat Background
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 10.0f, 20.0f }, { 0.75f, 0.3f, 0.3f, 1.0f });
}

void MusicLayer::OnAttach()
{

}

void MusicLayer::OnDetach()
{

}

void MusicLayer::OnUpdate(Hazel::Timestep ts)
{
	//Clear scene
	Hazel::RenderCommand::SetClearColor({ 0.08f, 0.08f, 0.08f, 1 });
	Hazel::RenderCommand::Clear();

	//Rendering scope
	{
		//Default is 57x57 at 1600x900
		Hazel::Renderer2D::BeginScene(*CameraController);
		CreateBeatArea();
		Hazel::Renderer2D::EndScene();
	}
	//1.0f moves camera 160 or 90 pixels in x or y direction respectively
	//0.01f is equal 9/16 for both ways
	//0.01f translation moves by 9/32 in x or y
	// 
	//Todo regulate framerate
	
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

	float camHeight = 8.0f;
	float bottom = -camHeight;
	float top = camHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	CameraController = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}