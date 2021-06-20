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
			:
			x("0.0f"),
			y("0.0f"),
			scaleX("1.0f"),
			scaleY("1.0f"),
			Colour(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
		}

		//Create quad from json object
		Quad::Quad(const json& object, const std::string& texturePath)
			:
			x(object["x"]),
			y(object["y"]),
			scaleX(object["scaleX"]),
			scaleY(object["scaleY"])
		{

			if (object["colour"].is_string())
			{
				Colour = Hazel::Texture2D::Create(texturePath + "/textures/" + std::string(object["colour"]));
			}
			else
			{
				Colour = Util::arrayToVec4(object["colour"]);
			}
		}

		Quad::Quad(ColourTexture Colour, const std::string& x, const std::string& y, const std::string& scaleX, const std::string& scaleY)
			:
			x(x),
			y(y),
			scaleX(scaleX),
			scaleY(scaleY),
			Colour(Colour)
		{
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
			catch (std::bad_variant_access const&)
			{
				Hazel::Ref<Hazel::Texture2D> pval = std::get<Hazel::Ref<Hazel::Texture2D>>(Colour);
				Hazel::Renderer2D::DrawQuad(
					{ getX() + xOffset, getY() + yOffset, z },
					toScaleVec(), pval);
			}
		}

		LayerSkin::LayerSkin(Quad BackgroundTexture, glm::vec4 ClearColour)
			:
			ClearColour(ClearColour),
			BackgroundTexture(BackgroundTexture)
		{
		}

		LayerSkin::LayerSkin(const json& object, const std::string& path) 
			:
			BackgroundTexture(Quad(object["Background"], path)),
			ClearColour(Util::arrayToVec4(object["ClearColour"]))
		{
		}

		LoadingSkin::LoadingSkin()
		{
		}

		LoadingSkin::LoadingSkin(Quad LoadingAnimation) : LoadingAnimation(LoadingAnimation)
		{
		}

		LoadingSkin::LoadingSkin(const json& object, const std::string& path)
			: 
			LayerSkin(object, path),
			LoadingAnimation(Quad(object["LoadingAnimation"], path))
		{
		}

		MusicSkin::MusicSkin()
		{

		}

		MusicSkin::MusicSkin(Quad Column1, Quad Column2, Quad Column3, Quad Column4, Quad Column5,
			Quad Beat, Quad BeatArea, Quad BeatZone)
			:
			Columns(std::vector<Quad>{ Column1, Column2, Column3, Column4, Column5 }),
			Beat(Beat),
			BeatArea(BeatArea),
			BeatZone(BeatZone)
		{
		}

		MusicSkin::MusicSkin(std::vector<Quad> Columns, Quad Beat, Quad BeatArea, Quad BeatZone)
			:			
			Columns(Columns),
			Beat(Beat),
			BeatArea(BeatArea),
			BeatZone(BeatZone)
		{
		}

		MusicSkin::MusicSkin(const json& object, const std::string& path)
			:
			LayerSkin(object, path),
			Beat(Quad(object["Beat"], path)),
			BeatArea(Quad(object["BeatArea"], path)),
			BeatZone(Quad(object["BeatZone"], path))
		{
			//Setup MusicSkin
			for (auto& column : object["Columns"])
			{
				Columns.push_back(Quad(column, path));
			}
			//TODO(Callum): Pass in texturePath from the skin.json not the skin path
		}

		AppSkin::AppSkin()
		{

		}

		AppSkin::AppSkin(const std::string& path)
		{
			//Find path
			std::string file;
			if (std::filesystem::is_regular_file(path))
			{
				
			}
			else if (std::filesystem::is_directory(path))
			{
				file = path + "/Skin.json";
			}
			else if (std::filesystem::is_directory(std::filesystem::current_path().string()
				+ "/assets/user/skins/" + path))
			{
				file = std::filesystem::current_path().string()
					+ "/assets/user/skins/" + path + "/Skin.json";
			}
			else
			{
				file = std::filesystem::current_path().string()
					+ "/assets/user/skins/Default/Skin.json";
			}

			std::ifstream input(file);
			json config;
			input >> config;
			input.close();

			SkinPath = path;
			SkinName = config["Name"];
			SkinDirectory = std::filesystem::path(file).parent_path().string();

			LoadingSkin = Skin::LoadingSkin(config["LoadingScreen"], SkinDirectory);
			MusicSkin = Skin::MusicSkin(config["MusicSkin"], SkinDirectory);
		}
	}
}