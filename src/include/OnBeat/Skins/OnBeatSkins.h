#pragma once
#include <Hazel.h>
#include <variant>
#include <vector>


namespace Skin
{
	typedef std::variant<glm::vec4, Hazel::Ref<Hazel::Texture2D>> ColourTexture;

	struct Quad
	{
		Quad();
		Quad(ColourTexture Colour,
			float x = 0.0f, float y = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f);

		float x, y, scaleX, scaleY;
		ColourTexture Colour;

		glm::vec2 toPositionVec();
	};

	struct DefaultSkin
	{
		DefaultSkin(Hazel::Ref<Hazel::Texture2D> BackgroundTexture = nullptr, glm::vec4 ClearColour = { 0.08f, 0.08f, 0.08f, 1 });

		glm::vec4 ClearColour;
		Hazel::Ref<Hazel::Texture2D> BackgroundTexture;
	};

	struct MusicSkin : DefaultSkin
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
		MusicSkin(std::vector<Quad>Columns, Quad Beat, Quad BeatArea);
		std::vector<Quad> Columns;
		Quad Beat;
		Quad BeatArea;
		Quad BeatZone;
	};
}