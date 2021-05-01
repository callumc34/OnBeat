#include <OnBeat/Config/OnBeatConfig.h>
#include <magic_enum/magic_enum.hpp>

using namespace Config;

glm::vec4 Config::arrayToVec4(json object)
{
	return glm::vec4(object[0], object[1], object[2], object[3]);
}

float Config::stringToFloat(std::string val, int flag)
{
	std::string type = val.substr(val.size() - 1, val.size());
	float n = std::atof(val.c_str());
	if (type == "%")
	{
		float scale;
		if (flag == HEIGHT)
		{
			scale = Hazel::Application::Get().GetWindow().GetHeight();
		}
		if (flag == WIDTH)
		{
			scale = Hazel::Application::Get().GetWindow().GetWidth();
		}
		return n * scale / 10000;
	}
	else if (type == "px")
	{
		float scale;
		if (flag == HEIGHT)
		{
			scale = Hazel::Application::Get().GetWindow().GetHeight();
		}
		if (flag == WIDTH)
		{
			scale = Hazel::Application::Get().GetWindow().GetWidth();
		}
		return n / 100;
	}
	else
	{
		return n;
	}
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
	x = object["x"];
	y = object["y"];
	scaleX = object["scaleX"];
	scaleY = object["scaleY"];

}

Skin::Quad::Quad(ColourTexture Colour, std::string x, std::string y, std::string scaleX, std::string scaleY)
{
	this->x = x;
	this->y = y;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	this->Colour = Colour;
}

float Config::Skin::Quad::getX()
{
	return stringToFloat(x, WIDTH);
}

float Config::Skin::Quad::getY()
{
	return stringToFloat(y, HEIGHT);
}
float Config::Skin::Quad::getScaleX()
{
	return stringToFloat(scaleX, WIDTH);
}
float Config::Skin::Quad::getScaleY()
{
	return stringToFloat(scaleY, HEIGHT);
}

glm::vec2 Skin::Quad::toScaleVec()
{
	return glm::vec2(
		stringToFloat(scaleX, WIDTH),
		stringToFloat(scaleY, HEIGHT)
	);
}

glm::vec2 Skin::Quad::toPositionVec()
{
	return glm::vec2(
		stringToFloat(x, WIDTH),
		stringToFloat(y, HEIGHT)
	);
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

Skin::MusicSkin::MusicSkin(json object, const char* path)
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
	ClearColour = arrayToVec4(object["ClearColour"]);
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

	MusicSkin = Skin::MusicSkin(config["MusicSkin"], path);

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

	Volume = config["Volume"];
}

