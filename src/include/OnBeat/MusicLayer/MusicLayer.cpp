#include <OnBeat/MusicLayer/MusicLayer.h>

MusicLayer::MusicLayer(OnBeat* app, AudioVector beats,
	float cameraVelocity, double sampleRate, int sampleSize)
	: Layer("MusicLayer")
{
	auto& window = app->GetWindow();
	//Class properties
	this->app = app;
	this->beats = OnSetGen::validateAudioVector(beats);
	//cameraVelocity in px/sec (900px/sec)
	this->cameraVelocity = cameraVelocity;
	//cameraVelocity in ratio px/sec (56.25px/sec)
	cameraVelocityRatio = OnBeat::pxToGlF(cameraVelocity);
	this->sampleRate = sampleRate;
	this->sampleSize = sampleSize;


	HZ_ASSERT(this->beats.size() != 0, "Invalid beats vector.");


	//Set up camera
	CreateCamera(window.GetWidth(), window.GetHeight());

	//Initialise shaders
	textureShader = shaderLibrary.Load("assets/shaders/Texture.glsl");
	flatColorShader = shaderLibrary.Load("assets/shaders/FlatColor.glsl");

	beatTexture = Hazel::Texture2D::Create("assets/textures/beat.png");

	FindBeatHeights();
}

void MusicLayer::CreateBeatArea()
{
	//Length = camera velocity * max time 
	//Todo render based on current camera position so it always renders the background
	//Todo turn all those 8s into window.width / 200
	float length = OnBeat::pxToGlF(cameraVelocity * ((sampleSize / sampleRate) * beats[0].size())) + 8.0f;

	float yOffset = cameraController.get()->GetPosition().y;
	//Lines
	Hazel::Renderer2D::DrawQuad({ -5.0f, (length / 2) - 8.0f }, { 0.1f, length }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ -2.5f, (length / 2) - 8.0f }, { 0.1f, length }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, (length / 2) - 8.0f }, { 0.1f, length }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 2.5f, (length / 2) - 8.0f }, { 0.1f, length }, { 1.0f, 1.0f, 1.0f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 5.0f, (length / 2) - 8.0f }, { 0.1f, length }, { 1.0f, 1.0f, 1.0f, 1.0f });
	//Beat Background
	Hazel::Renderer2D::DrawQuad({ 0.0f, yOffset }, { 10.0f, OnBeat::pxToGlF(1600.0f) }, { 0.75f, 0.3f, 0.3f, 1.0f });
}

void MusicLayer::CreateBeats()
{
	//Get heights of blits
	for (const auto& [key, vector] : blitHeights)
	{
		for (auto& value : vector)
		{
			glm::vec2 position;
			if (key == 1)
			{
				position = { COLUMN_1_X, value };
			}
			else if (key == 2)
			{
				position = { COLUMN_2_X, value };
			}
			else if (key == 3)
			{
				position = { COLUMN_3_X, value };
			}
			else
			{
				position = { COLUMN_4_X, value };
			}

			Hazel::Renderer2D::DrawQuad(position,
				{ OnBeat::pxToGlF(beatTexture.get()->GetWidth()), OnBeat::pxToGlF(beatTexture.get()->GetHeight()) }, beatTexture);

		}
		
	}
}

void MusicLayer::FindBeatHeights()
{
	//Blit y = Camera velocity * time of blit * 1/2(Blit height)
	//Possible adjustment needed to represent middle of sample size however sample size so small likely unneccessary
	double threshold = 0.5;
	double time = sampleSize / sampleRate;
	for (int c = 0; c < beats.size(); c++)
	{
		int opt1, opt2;
		if (c == 0)
		{
				opt1 = 1;
				opt2 = 2;
		}
		else
		{
			opt1 = 3;
			opt2 = 4;
		}

		for (int n = 0; n < beats[c].size(); n++)
		{
			blitHeights[(beats[c][n] > threshold) ? opt1 : opt2].push_back(100 * cameraVelocity * time + 0.5f);
		}
	}

}

void MusicLayer::OnAttach()
{

}

void MusicLayer::OnDetach()
{

}

void MusicLayer::OnUpdate(Hazel::Timestep ts)
{
	//Todo Keep track of time and end scene when done
	// 
	// 
	//Clear scene
	Hazel::RenderCommand::SetClearColor({ 0.08f, 0.08f, 0.08f, 1 });
	Hazel::RenderCommand::Clear();

	//Rendering scope
	{
		//Default is 56.25x56.25 at 1600x900
		Hazel::Renderer2D::BeginScene(*cameraController);
		CreateBeats();
		CreateBeatArea();
		Hazel::Renderer2D::EndScene();
	}
	//1.0f moves camera 1600( i think ) or 900 pixels in x or y direction respectively
	//0.01f is equal 9/16 for both ways
	 
	
	//Todo regulate framerate
	glm::vec3 cameraPos = cameraController.get()->GetPosition();
	cameraPos.y += cameraVelocityRatio * ts.GetSeconds();
	cameraController.get()->SetPosition(cameraPos);
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

	float camHeight = height / 200;
	float bottom = -camHeight;
	float top = camHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	cameraController = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}