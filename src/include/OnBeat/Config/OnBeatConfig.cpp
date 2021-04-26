#include <OnBeat/Config/OnBeatConfig.h>
#include <magic_enum/magic_enum.hpp>

using namespace Config;

Settings::Settings() 
{

}

//Setup OnBeat settings from a config.json
Settings::Settings(const char* path) 
{
	std::ifstream input(path);
	json config;
	input >> config;
	input.close();

	DisplayWidth = config["Display"][0];
	DisplayHeight = config["Display"][1];

	for (auto& [key, value] : config["Input"].items())
	{
		auto inputKey = magic_enum::enum_cast<Keys>(std::string(value));
		if (inputKey.has_value())
		{
			Input[key] = inputKey.value();
		}
		else
		{
			//Todo error checking and defaulting
		}
	}

	CurrentSkinPath = std::filesystem::current_path().string()
		+ "/assets/user/skins/" + std::string(config["CurrentSkin"]);
	CurrentSkin = Skin::OnBeatSkin(CurrentSkinPath.c_str());

}

Skin::Quad::Quad()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->scaleX = 1.0f;
	this->scaleY = 1.0f;
	this->Colour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
}

//Create quad from json object
Skin::Quad::Quad(json object, std::string texturePath)
{

	if (object["colour"].is_string())
	{
		Colour = Hazel::Texture2D::Create(texturePath + "/textures/" + std::string(object["colour"]));
	}
	else
	{
		Colour = glm::vec4(object["colour"][0], object["colour"][1], object["colour"][2], object["colour"][3]);
	}

	x = object["x"];
	y = object["y"];
	scaleX = object["scaleX"];
	scaleY = object["scaleY"];

}

Skin::Quad::Quad(ColourTexture Colour, float x, float y, float scaleX, float scaleY)
{
	this->x = x;
	this->y = y;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->Colour = Colour;
}

glm::vec2 Skin::Quad::toPositionVec()
{
	return glm::vec2(x, y);
}

Skin::DefaultSkin::DefaultSkin(Hazel::Ref<Hazel::Texture2D> BackgroundTexture, glm::vec4 ClearColour)
{
	this->ClearColour = ClearColour;
	this->BackgroundTexture = BackgroundTexture;
}

Skin::MusicSkin::MusicSkin()
{

}

Skin::MusicSkin::MusicSkin(Quad Column1, Quad Column2, Quad Column3, Quad Column4, Quad Column5,
	Quad Beat, Quad BeatArea, Quad BeatZone)
{
	this->Columns = std::vector<Quad>{ Column1, Column2, Column3, Column4, Column5 };
	this->Beat = Beat;
	this->BeatArea = BeatArea;
	this->BeatZone = BeatZone;
}

Skin::MusicSkin::MusicSkin(std::vector<Quad> Columns, Quad Beat, Quad BeatArea, Quad BeatZone)
{
	this->Columns = Columns;
	this->Beat = Beat;
	this->BeatArea = BeatArea;
	this->BeatZone = BeatZone;
}

Skin::OnBeatSkin::OnBeatSkin()
{

}

Skin::OnBeatSkin::OnBeatSkin(const char* path)
{
	std::ifstream input(std::string(path) + "/Skin.json");
	json config;
	input >> config;
	input.close();

	SkinPath = path;

	//Setup MusicSkin
	std::vector<Skin::Quad> Columns;
	for (auto& column : config["MusicSkin"]["Columns"])
	{
		Columns.push_back(Skin::Quad(column, path));
	}

	MusicSkin = Skin::MusicSkin(Columns, Skin::Quad(config["MusicSkin"]["Beat"], path),
		Skin::Quad(config["MusicSkin"]["BeatArea"], path), Skin::Quad(config["MusicSkin"]["BeatZone"], path));

}
