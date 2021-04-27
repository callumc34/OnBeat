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

	this->sampleRate = sampleRate;
	this->sampleSize = sampleSize;


	HZ_ASSERT(this->beats.size() != 0, "Invalid beats vector.");

	//Set up camera
	CreateCamera(window.GetWidth(), window.GetHeight());

	skin = app->Settings.CurrentSkin.MusicSkin;

	FindBeatHeights();
}

void MusicLayer::CreateBeatArea()
{
	//Needs to be cleaned up
	auto& window = app->GetWindow();

	float length = window.GetHeight() / 100.0f;

	float yOffset = cameraController.get()->GetPosition().y;
	float columnOffset;
	glm::vec2 position = skin.BackgroundTexture.toPositionVec();
	//Background
	try
	{
		glm::vec4 pval = std::get<glm::vec4>(skin.BackgroundTexture.Colour);
		Hazel::Renderer2D::DrawQuad(
			{ position.x, position.y + yOffset, -0.5f },
			skin.BackgroundTexture.toScaleVec(), pval);
	}
	catch (std::bad_variant_access const& ex)
	{
		Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(skin.BackgroundTexture.Colour);
		Hazel::Renderer2D::DrawQuad(
			{ position.x, position.y + yOffset, -0.5f },
			skin.BackgroundTexture.toScaleVec(), pval);
	}

	//Beat Columns
	for (auto& column : skin.Columns)
	{
		try
		{
			glm::vec4 pval = std::get<glm::vec4>(column.Colour);
			Hazel::Renderer2D::DrawQuad({ column.toPositionVec()[0], yOffset, -0.1f }, { column.toScaleVec()[0], length }, pval);
		}
		catch (std::bad_variant_access const& ex)
		{
			Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(column.Colour);
			Hazel::Renderer2D::DrawQuad({ column.toPositionVec()[0], yOffset, -0.1f }, { column.toScaleVec()[0], length }, pval);
		}
	}

	//Beat Background
	try
	{
		glm::vec4 pval = std::get<glm::vec4>(skin.BeatArea.Colour);
		Hazel::Renderer2D::DrawQuad({ skin.BeatArea.toPositionVec()[0], yOffset, -0.15f }, { skin.BeatArea.toScaleVec()[0], length }, pval);
	}
	catch (std::bad_variant_access const& ex)
	{
		Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(skin.BeatArea.Colour);
		Hazel::Renderer2D::DrawQuad({ skin.BeatArea.toPositionVec()[0], yOffset, -0.15f }, { skin.BeatArea.toScaleVec()[0], length }, pval);
	}

	//Beat Zone
	position = skin.BeatZone.toPositionVec();
	auto test = skin.BeatZone.toScaleVec();
	try
	{
		glm::vec4 pval = std::get<glm::vec4>(skin.BeatZone.Colour);
		Hazel::Renderer2D::DrawQuad(
			{ position.x, yOffset + position.y - OnBeat::pxToGlF(app->GetWindow().GetHeight() / 2), 0.2f },
			skin.BeatZone.toScaleVec(), pval);
	}
	catch (std::bad_variant_access const& ex)
	{
		Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(skin.BeatZone.Colour);
		Hazel::Renderer2D::DrawQuad(
			{ position.x, yOffset + position.y - OnBeat::pxToGlF(app->GetWindow().GetHeight() / 2), 0.2f },
			skin.BeatZone.toScaleVec(), pval);
	}
}

void MusicLayer::CreateBeats()
{
	//Iterate through timing vector and create any within two windows up one window down
	float beatTextureHeight, beatTextureWidth;
	try
	{
		Hazel::Ref<Hazel::Texture2D> beatTexture = std::get<Hazel::Ref<Hazel::Texture2D>>(skin.Beat.Colour);
		beatTextureHeight = OnBeat::pxToGlF(beatTexture.get()->GetHeight());
		beatTextureWidth = OnBeat::pxToGlF(beatTexture.get()->GetWidth());

	}
	catch (std::bad_variant_access const& ex)
	{
		glm::vec2 beatTexture = skin.Beat.toScaleVec();
		beatTextureHeight = beatTexture[1];
		beatTextureWidth = beatTexture[0];
	}
	for (const auto& [key, vector] : beatHeights)
	{
		float zIndex = 0.01f;
		int index = 0;
		for (auto& value : vector)
		{
			if (value > cameraController.get()->GetPosition().y + beatTextureHeight + 2*OnBeat::pxToGlF(app->GetWindow().GetHeight()))
			{
				break;
			}
			else if (value < cameraController.get()->GetPosition().y - OnBeat::pxToGlF(app->GetWindow().GetHeight()))
			{
				continue;
			}

			Config::Skin::Quad column = skin.Columns[key - 1];

			float columnX = column.toPositionVec()[0];

			float scale = std::abs(columnX - skin.Columns[key].toPositionVec()[0]) / beatTextureWidth;

			glm::vec2 size(scale * beatTextureWidth,
				scale * beatTextureHeight);

			glm::vec3 position(
				columnX + size[0]/2, value - OnBeat::pxToGlF(app->GetWindow().GetHeight()/2) + skin.BeatZone.toPositionVec()[1], zIndex);

			try
			{
				Hazel::Renderer2D::DrawQuad(position, size, std::get<Hazel::Ref<Hazel::Texture2D>>(skin.Beat.Colour));
			}
			catch (std::bad_variant_access const& ex)
			{
				Hazel::Renderer2D::DrawQuad(position, size, std::get<glm::vec4>(skin.Beat.Colour));
			}
			zIndex += 0.001f;
		}
		
	}
}

void MusicLayer::FindBeatHeights()
{
	//Blit y = Camera velocity * time of blit
	//Possible adjustment needed to represent middle of sample size however sample size so small likely unneccessary
	double time = (sampleSize) / sampleRate;
	for (int c = 0; c < beats.size(); c++)
	{
		double threshold = OnSetGen::findPeakThreshold(beats[c]);
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
		Hazel::RenderCommand::SetClearColor(skin.ClearColour);
		Hazel::RenderCommand::Clear();
	}

	//Rendering scope
	{
		Hazel::Renderer2D::BeginScene(*cameraController);
		//Draw background
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
	Hazel::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(MusicLayer::OnWindowResize));
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