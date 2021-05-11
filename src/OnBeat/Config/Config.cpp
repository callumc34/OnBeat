#include <OnBeat/Config/Config.h>
//#include <magic_enum.hpp>
#include <filesystem>
#include <fstream>

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

			for (auto& [key, value] : config["Input"].items())
			{
				//auto inputKey = magic_enum::enum_cast<Keys>(std::string(value));
				//if (inputKey.has_value())
				//{
				//	Input[key] = inputKey.value();
				//}
				//else
				//{
				//	//Todo error checking and defaulting
				//}
				uint16_t keyCode = (uint16_t)value;
			}

			CurrentSkinPath = std::filesystem::current_path().string()
				+ "/assets/user/skins/" + std::string(config["CurrentSkin"]);
			CurrentSkin = Skin::AppSkin(CurrentSkinPath.c_str());

			Volume = config["Volume"];
		}
	}
}