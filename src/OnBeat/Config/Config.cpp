#include <OnBeat/Config/Config.h>
#include <magic_enum.hpp>
#include <filesystem>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

namespace OnBeat
{
	namespace Config
	{
		//Setup onbeat settings from json file
		Settings::Settings(const char* path)
		{
			std::ifstream input(path);
			json config;
			input >> config;
			input.close();

			DisplayWidth = config["Display"][0];
			DisplayHeight = config["Display"][1];
			Fullscreen = config["Fullscreen"];

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

			std::string skinPath = std::filesystem::current_path().string()
				+ "/assets/user/skins/" + std::string(config["CurrentSkin"]) + "/Skin.json";
			CurrentSkin = Skin::AppSkin(skinPath);

			Volume = config["Volume"];
		}

		void to_json(json& j, const Settings& s)
		{
			j["Resolution"] = std::to_string(s.DisplayWidth) + "x" + std::to_string(s.DisplayHeight);
			j["Fullscreen"] = s.Fullscreen;
			j["Skin"] = s.CurrentSkin.SkinPath;
			j["Volume"] = s.Volume * 100;
		}

		void from_json(const json& j, Settings& s)
		{
			std::string resString = j["Resolution"];
			s.DisplayWidth = std::stoi(resString.substr(0, resString.find("x", 0)).c_str());
			s.DisplayHeight = std::stoi(resString.substr(resString.find("x", 0) + 1, resString.length()).c_str());
			j.at("Fullscreen").get_to(s.Fullscreen);
			s.CurrentSkin = Skin::AppSkin(j["Skin"]);
			s.Volume = j["Volume"] / 100;
		}
	}
}