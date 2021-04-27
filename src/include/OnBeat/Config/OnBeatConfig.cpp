#include <OnBeat/Config/OnBeatConfig.h>
#include <magic_enum/magic_enum.hpp>

using namespace Config;

glm::vec4 Config::arrayToVec4(json object)
{
	return glm::vec4(object[0], object[1], object[2], object[3]);
}

float Config::percentageToFloat(std::string percentage, float val)
{
	float perc = std::atof(percentage.substr(0, percentage.size() - 1).c_str());
	return perc / 10000 * val;
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
		Colour = arrayToVec4(object["colour"]);
	}

	for (auto& value : object["dimensions"])
	{

	}

	//Bit of a mess may need cleaning up
	x = (object["x"].is_string()) ?
		percentageToFloat(object["x"], Hazel::Application::Get().GetWindow().GetWidth()) : (float)object["x"];

	y = (object["y"].is_string()) ?
		percentageToFloat(object["y"], Hazel::Application::Get().GetWindow().GetHeight()) : (float)object["y"];

	scaleX = (object["scaleX"].is_string()) ?
		percentageToFloat(object["scaleX"], Hazel::Application::Get().GetWindow().GetWidth()) : (float)object["scaleX"];

	scaleY = (object["scaleY"].is_string()) ?
		percentageToFloat(object["scaleY"], Hazel::Application::Get().GetWindow().GetHeight()) : (float)object["scaleY"];

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

Skin::LayerSkin::LayerSkin(Quad BackgroundTexture, glm::vec4 ClearColour)
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
	MusicSkin.BackgroundTexture = Quad(config["MusicSkin"]["Background"], path);
	MusicSkin.ClearColour = arrayToVec4(config["MusicSkin"]["ClearColour"]);

}
