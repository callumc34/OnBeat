#include <OnBeat/Skins/OnBeatSkins.h>

using namespace Skin;

Quad::Quad()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->Colour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
}

Quad::Quad(ColourTexture Colour, float x, float y, float scaleX, float scaleY)
{
	this->x = x;
	this->y = y;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->Colour = Colour;
}

glm::vec2 Quad::toPositionVec()
{
	return glm::vec2(x, y);
}

DefaultSkin::DefaultSkin(Hazel::Ref<Hazel::Texture2D> BackgroundTexture, glm::vec4 ClearColour)
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

MusicSkin::MusicSkin(std::vector<Quad> Columns, Quad Beat, Quad BeatArea)
{
	this->Columns = Columns;
	this->Beat = Beat;
	this->BeatArea = BeatArea;
}
