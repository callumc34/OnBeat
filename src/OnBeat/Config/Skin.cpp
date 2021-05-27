#include <OnBeat/Config/Skin.h>
#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <Hazel/Renderer/Renderer2D.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace OnBeat
{
	namespace Skin
	{
		Quad::Quad()
		{
			this->x = 0.0f;
			this->y = 0.0f;
			this->scaleX = 1.0f;
			this->scaleY = 1.0f;
			this->Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//Create quad from json object
		Quad::Quad(json object, std::string texturePath)
		{

			if (object["colour"].is_string())
			{
				Colour = Hazel::Texture2D::Create(texturePath + "/textures/" + std::string(object["colour"]));
			}
			else
			{
				Colour = Util::arrayToVec4(object["colour"]);
			}

			for (auto& value : object["dimensions"])
			{

			}

			//Bit of a mess may need cleaning up
			x = object["x"];
			y = object["y"];
			scaleX = object["scaleX"];
			scaleY = object["scaleY"];

		}

		Quad::Quad(ColourTexture Colour, std::string x, std::string y, std::string scaleX, std::string scaleY)
		{
			this->x = x;
			this->y = y;
			this->scaleX = scaleX;
			this->scaleY = scaleY;
			this->Colour = Colour;
		}

		float Quad::getX()
		{
			return Util::stringSizeToFloat(x, Util::Flag::WIDTH);
		}

		float Quad::getY()
		{
			return Util::stringSizeToFloat(y, Util::Flag::HEIGHT);
		}
		float Quad::getScaleX()
		{
			return Util::stringSizeToFloat(scaleX, Util::Flag::WIDTH);
		}
		float Quad::getScaleY()
		{
			return Util::stringSizeToFloat(scaleY, Util::Flag::HEIGHT);
		}

		glm::vec2 Quad::toScaleVec()
		{
			return glm::vec2(
				Util::stringSizeToFloat(scaleX, Util::Flag::WIDTH),
				Util::stringSizeToFloat(scaleY, Util::Flag::HEIGHT)
			);
		}

		glm::vec2 Quad::toPositionVec()
		{
			return glm::vec2(
				Util::stringSizeToFloat(x, Util::Flag::WIDTH),
				Util::stringSizeToFloat(y, Util::Flag::HEIGHT)
			);
		}

		void Quad::draw(float z, float xOffset, float yOffset)
		{
			try
			{
				glm::vec4 pval = std::get<glm::vec4>(Colour);
				Hazel::Renderer2D::DrawQuad(
					{ getX() + xOffset, getY() + yOffset, z },
					toScaleVec(), pval);
			}
			catch (std::bad_variant_access const& ex)
			{
				Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(Colour);
				Hazel::Renderer2D::DrawQuad(
					{ getX() + xOffset, getY() + yOffset, z },
					toScaleVec(), pval);
			}
		}

		LayerSkin::LayerSkin(Quad BackgroundTexture, glm::vec4 ClearColour)
		{
			this->ClearColour = ClearColour;
			this->BackgroundTexture = BackgroundTexture;
		}

		MusicSkin::MusicSkin()
		{

		}

		MusicSkin::MusicSkin(Quad Column1, Quad Column2, Quad Column3, Quad Column4, Quad Column5,
			Quad Beat, Quad BeatArea, Quad BeatZone)
		{
			this->Columns = std::vector<Quad>{ Column1, Column2, Column3, Column4, Column5 };
			this->Beat = Beat;
			this->BeatArea = BeatArea;
			this->BeatZone = BeatZone;
		}

		MusicSkin::MusicSkin(std::vector<Quad> Columns, Quad Beat, Quad BeatArea, Quad BeatZone)
		{
			this->Columns = Columns;
			this->Beat = Beat;
			this->BeatArea = BeatArea;
			this->BeatZone = BeatZone;
		}

		MusicSkin::MusicSkin(json object, std::string path)
		{
			//Setup MusicSkin
			for (auto& column : object["Columns"])
			{
				Columns.push_back(Quad(column, path));
			}

			Beat = Quad(object["Beat"], path);
			BeatArea = Quad(object["BeatArea"], path);
			BeatZone = Quad(object["BeatZone"], path);
			BackgroundTexture = Quad(object["Background"], path);
			ClearColour = Util::arrayToVec4(object["ClearColour"]);
		}

		AppSkin::AppSkin()
		{

		}

		AppSkin::AppSkin(std::string path)
		{
			std::ifstream input(path);
			json config;
			input >> config;
			input.close();

			SkinPath = path;
			SkinName = config["Name"];

			SkinDirectory = std::filesystem::path(path).parent_path().string();

			MusicSkin = Skin::MusicSkin(config["MusicSkin"], SkinDirectory);
		}
	}
}