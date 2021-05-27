#pragma once
#include <Hazel/Renderer/Texture.h>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>
#include <variant>

namespace OnBeat
{
	namespace Skin
	{
		typedef std::variant<glm::vec4, Hazel::Ref<Hazel::Texture2D>> ColourTexture;

		struct Quad
		{
			Quad();
			Quad(nlohmann::json object, std::string texturePath);
			Quad(ColourTexture Colour,
				std::string x = "0.0f", std::string y = "0.0f", std::string scaleX = "1.0f", std::string scaleY = "1.0f");

			std::string x, y, scaleX, scaleY;
			ColourTexture Colour;

			float getX();
			float getY();
			float getScaleX();
			float getScaleY();

			glm::vec2 toScaleVec();
			glm::vec2 toPositionVec();

			void draw(float z = 0.0f, float xOffset = 0.0f, float yOffset = 0.0f);
		};

		struct LayerSkin
		{
			LayerSkin(Quad BackgroundTexture = Quad(), glm::vec4 ClearColour = { 0.08f, 0.08f, 0.08f, 1 });

			glm::vec4 ClearColour;
			Quad BackgroundTexture;
		};

		struct MusicSkin : LayerSkin
		{
			MusicSkin();
			MusicSkin(
				Quad Column1,
				Quad Column2,
				Quad Column3,
				Quad Column4,
				Quad Column5,
				Quad Beat,
				Quad BeatArea,
				Quad BeatZone);
			MusicSkin(std::vector<Quad>Columns, Quad Beat, Quad BeatArea, Quad BeatZone);
			MusicSkin(nlohmann::json object, std::string path);

			std::vector<Quad> Columns;
			Quad Beat;
			Quad BeatArea;
			Quad BeatZone;
		};

		struct AppSkin
		{
			AppSkin();
			AppSkin(std::string path);

			MusicSkin MusicSkin;
			std::string SkinPath;
			std::string SkinName;
			std::string SkinDirectory;
		};
	}
}