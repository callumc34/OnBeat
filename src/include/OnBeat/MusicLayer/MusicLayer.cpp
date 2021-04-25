#include <OnBeat/MusicLayer/MusicLayer.h>
#include <GLFW/glfw3.h>

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
	this->cameraVelocityRatio = OnBeat::pxToGlF(cameraVelocity);
	//cameraVelocity in ratio px/sec (56.25px/sec)
	this->sampleRate = sampleRate;
	this->sampleSize = sampleSize;


	HZ_ASSERT(this->beats.size() != 0, "Invalid beats vector.");

	//Set up camera
	CreateCamera(window.GetWidth(), window.GetHeight());

	//Initialise shaders
	beatTexture = Hazel::Texture2D::Create("assets/textures/beat.png");

	//Generic Default skin - Todo move this into a json and create parser
	Skin::Quad column(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	column.scaleX = 0.1f;

	auto column1 = column;
	column1.x = -5.0f;
	column1.scaleX = 0.1f;
	auto column2 = column;
	column2.x = -2.5f;
	column2.scaleX = 0.1f;
	auto column3 = column;
	column3.x = 0.0f;
	column3.scaleX = 0.1f;
	auto column4 = column;
	column4.x = 2.5f;
	column4.scaleX = 0.1f;
	auto column5 = column;
	column5.x = 5.0f;
	column5.scaleX = 0.1f;

	currentSkin = Skin::MusicSkin(column1, column2, column3, column4, column5,
		Skin::Quad(beatTexture), Skin::Quad(glm::vec4( 0.75f, 0.3f, 0.3f, 1.0f ), 0.0f, 0.0f, 10.0f));

	FindBeatHeights();
}

void MusicLayer::CreateBeatArea()
{
	//Needs to be cleaned up
	auto& window = app->GetWindow();

	float length = window.GetHeight() / 100;

	float yOffset = cameraController.get()->GetPosition().y;
	float columnOffset;
	//Lines
	for (auto& column : currentSkin.Columns)
	{
		try
		{
			glm::vec4 pval = std::get<glm::vec4>(column.Colour);
			Hazel::Renderer2D::DrawQuad({ column.x, yOffset, -0.1f }, { column.scaleX, length }, pval);
		}
		catch (std::bad_variant_access const& ex)
		{
			Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(column.Colour);
			Hazel::Renderer2D::DrawQuad({ column.x, yOffset, -0.1f }, { column.scaleX, length }, pval);
		}
	}

	//Beat Background
	try
	{
		glm::vec4 pval = std::get<glm::vec4>(currentSkin.BeatArea.Colour);
		Hazel::Renderer2D::DrawQuad({ currentSkin.BeatArea.x, yOffset, -0.15f }, { currentSkin.BeatArea.scaleX, length }, pval);
	}
	catch (std::bad_variant_access const& ex)
	{
		Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(currentSkin.BeatArea.Colour);
		Hazel::Renderer2D::DrawQuad({ currentSkin.BeatArea.x, yOffset, -0.15f }, { currentSkin.BeatArea.scaleX, length }, pval);
	}
}

void MusicLayer::CreateBeats()
{
	//Iterate through timing vector and create any within two windows up one window down
	for (const auto& [key, vector] : beatHeights)
	{
		float zIndex = 0.01f;
		int index = 0;
		for (auto& value : vector)
		{
			if (value > cameraController.get()->GetPosition().y + OnBeat::pxToGlF(beatTexture.get()->GetHeight()) + 2*OnBeat::pxToGlF(app->GetWindow().GetHeight()))
			{
				break;
			}
			else if (value < cameraController.get()->GetPosition().y - OnBeat::pxToGlF(app->GetWindow().GetHeight()))
			{
				continue;
			}

			Skin::Quad column = currentSkin.Columns[key - 1];

			float scale = std::abs(column.x - currentSkin.Columns[key].x) / OnBeat::pxToGlF(beatTexture.get()->GetWidth());

			glm::vec2 size(OnBeat::pxToGlF(scale * beatTexture.get()->GetWidth()),
				OnBeat::pxToGlF(scale * beatTexture.get()->GetHeight()));

			glm::vec3 position(column.x + size[0]/2, value + size[1]/2 - OnBeat::pxToGlF(app->GetWindow().GetHeight()/2), zIndex);

			Hazel::Renderer2D::DrawQuad(position, size, beatTexture);
			zIndex += 0.001f;
		}
		
	}
}

void MusicLayer::FindBeatHeights()
{
	//Blit y = Camera velocity * time of blit
	//Possible adjustment needed to represent middle of sample size however sample size so small likely unneccessary
	double threshold = 0.5;
	double time = (sampleSize) / sampleRate;
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
			if (beats[c][n] == 0)
			{
				continue;
			}
			beatHeights[(beats[c][n] > threshold) ? opt1 : opt2].push_back(cameraVelocityRatio * time * (n+1));
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
	Hazel::Renderer2D::ResetStats();
	{
		Hazel::RenderCommand::SetClearColor({ 0.08f, 0.08f, 0.08f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	//Rendering scope
	{
		Hazel::Renderer2D::BeginScene(*cameraController);
		CreateBeatArea();
		CreateBeats();
		Hazel::Renderer2D::EndScene();
	}	 
	
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

	float camHeight = (float)height / 200;
	float bottom = -camHeight;
	float top = camHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	cameraController = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
}