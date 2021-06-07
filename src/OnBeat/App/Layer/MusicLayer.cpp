#include <OnBeat/App/Layer/MusicLayer.h>
#include <OnBeat/App/App.h>
#include <Hazel/Renderer/RenderCommand.h>

namespace OnBeat {
	MusicLayer::MusicLayer(const char* file,
		float cameraVelocity, double sampleRate, int sampleSize)
		: Layer("MusicLayer"), beatGen({ 0, 1, 8, 8 }, file)
	{
		auto& window = App::Get().GetWindow();

		//cameraVelocity in px/sec (900px/sec)
		this->cameraVelocity = cameraVelocity;
		this->cameraVelocityRatio = cameraVelocity / 100.0f;

		this->sampleRate = sampleRate;
		this->sampleSize = sampleSize;

		skin = App::Get().Settings.CurrentSkin.MusicSkin;

		musicFile = file;
		App::Get().AudioPlayer.loadAudio(file);

		beats = beatGen.findBeats(beatGen.processFile());

		HZ_ASSERT(beats.size() != 0, "Invalid beats vector.");

		//Set up camera
		CreateCamera(window.GetWidth(), window.GetHeight());

		FindBeatHeights();
	}

	void MusicLayer::CreateBeatArea()
	{
		//Needs to be cleaned up
		auto& window = App::Get().GetWindow();

		float length = window.GetHeight() / 100.0f;

		float yOffset = CameraController.get()->GetPosition().y;

		glm::vec2 position = skin.BackgroundTexture.toPositionVec();
		//Background
		skin.BackgroundTexture.draw(-0.5f, position.x, position.y + yOffset);

		//Beat Columns
		for (auto& column : skin.Columns)
		{
			column.draw(-0.1f, 0.0f, yOffset);
		}

		//Beat Background
		skin.BeatArea.draw(-0.15f, 0.0f, yOffset);

		//Beat Zone
		skin.BeatZone.draw(0.2f, 0.0f, yOffset + position.y - App::Get().GetWindow().GetHeight() / 200);
	}

	void MusicLayer::CreateBeats()
	{
		//Iterate through timing vector and create any within two windows up one window down
		float beatTextureHeight, beatTextureWidth;
		try
		{
			Hazel::Ref<Hazel::Texture2D> beatTexture = std::get<Hazel::Ref<Hazel::Texture2D>>(skin.Beat.Colour);
			beatTextureHeight = beatTexture.get()->GetHeight() / 100.0f;
			beatTextureWidth = beatTexture.get()->GetWidth() / 100.0f;

		}
		catch (std::bad_variant_access const&)
		{
			glm::vec2 beatTexture = skin.Beat.toScaleVec();
			beatTextureHeight = beatTexture[1];
			beatTextureWidth = beatTexture[0];
		}
		for (const auto& [key, vector] : beatHeights)
		{
			float zIndex = 0.01f;
			int index = 0;

			//Calculate beat size per column
			Skin::Quad column = skin.Columns[key - 1];

			float scale = std::abs(column.getX() - skin.Columns[key].getX()) / beatTextureWidth;
			skin.Beat.scaleX = std::to_string(scale * beatTextureWidth);
			skin.Beat.scaleY = std::to_string(scale * beatTextureHeight);

			float offsetX = column.getX() + (scale * beatTextureWidth / 2);
			float offsetY = (App::Get().GetWindow().GetHeight() / 200) + skin.BeatZone.getY();

			for (auto& value : vector)
			{
				if (value > CameraController.get()->GetPosition().y + beatTextureHeight + (2 * App::Get().GetWindow().GetHeight() / 100.0f))
				{
					break;
				}
				else if (value < CameraController.get()->GetPosition().y - (App::Get().GetWindow().GetHeight() / 100.0f))
				{
					continue;
				}



				//Draw beats
				//Cannot use quad draw due to render time calculating string to float
				try
				{
					Hazel::Renderer2D::DrawQuad(
						{ offsetX, value - offsetY, zIndex },
						{ scale * beatTextureWidth, scale * beatTextureHeight }, std::get<Hazel::Ref<Hazel::Texture2D>>(skin.Beat.Colour));
				}
				catch (std::bad_variant_access const&)
				{
					Hazel::Renderer2D::DrawQuad(
						{ offsetX, value - offsetY, zIndex },
						{ scale * beatTextureWidth, scale * beatTextureHeight }, std::get<glm::vec4>(skin.Beat.Colour));
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
			double threshold = OnSetDetection::findPeakThreshold(beats[c]);
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
				beatHeights[(beats[c][n] > threshold) ? opt1 : opt2].push_back(cameraVelocityRatio * (float)time * (n+1));
			}
		}

	}

	void MusicLayer::OnAttach()
	{
		App::Get().AudioPlayer.playAudio();
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
			Hazel::Renderer2D::BeginScene(*CameraController);
			//Draw background
			CreateBeatArea();
			CreateBeats();
			Hazel::Renderer2D::EndScene();
		}	 
	
		//Todo regulate framerate
		glm::vec3 cameraPos = CameraController.get()->GetPosition();
		cameraPos.y += cameraVelocityRatio * ts.GetSeconds();
		CameraController.get()->SetPosition(cameraPos);
	}

	void MusicLayer::EventUpdate(Hazel::Event& e)
	{
	}

	void MusicLayer::OnImGuiRender()
	{
	}
}