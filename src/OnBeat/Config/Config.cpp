#include <OnBeat/App/App.h>
#include <OnBeat/Config/Config.h>
#include <magic_enum.hpp>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;

namespace OnBeat
{
	namespace Config
	{
		Settings::Settings(const std::string& path)
		{
			std::ifstream input(path);
			json config;
			input >> config;
			input.close();

			Resolution = { config["Display"][0], config["Display"][1], config["Fullscreen"], config["FpsCap"] };

			Volume = config["Volume"];

			for (auto& [key, value] : config["Input"].items())
			{
				auto inputKey = magic_enum::enum_cast<Keys>(std::string(value));
				if (inputKey.has_value())
				{
					Inputs[key] = inputKey.value();
				}
				else
				{
					//Todo error checking and defaulting
				}
			}
			
			//Try to find skin file
			std::string skin = config["Skin"];

			if (std::filesystem::is_regular_file(skin))
			{
				CurrentSkin = Skin::AppSkin(skin);
			}
			else if (std::filesystem::is_directory(skin))
			{
				CurrentSkin = Skin::AppSkin(skin + "/Skin.json");
			}
			else if (std::filesystem::is_directory(std::filesystem::current_path().string()
				+ "/assets/user/skins/" + skin))
			{
				CurrentSkin = Skin::AppSkin(std::filesystem::current_path().string()
					+ "/assets/user/skins/" + skin + "/Skin.json");
			}
			else
			{
				CurrentSkin = Skin::AppSkin(std::filesystem::current_path().string()
					+ "/assets/user/skins/Default/Skin.json");
			}
		
		}

		void Settings::setResolution(const ResolutionData& resolution)
		{ 
			this->Resolution = resolution;
			App::Get().SetWindowState(Resolution.Fullscreen);
		}

		void Settings::setVolume(const float& volume)
		{
			this->Volume = volume;
			App::Get().AudioPlayer.setVolume(Volume);
		}

		void Settings::setInputs(const InputMap& inputs)
		{ 
			this->Inputs = inputs;
		}

		void Settings::setCurrentSkin(const Skin::AppSkin& skin)
		{ 
			this->CurrentSkin = skin;
		}

		Settings& Settings::operator=(const json& j)
		{
			std::string resString = j["Resolution"];
			ResolutionData res = { 0, 0, j["Fullscreen"], j["FpsCap"] };
			res.DisplayWidth = std::stoi(resString.substr(0, resString.find("x", 0)).c_str());
			res.DisplayHeight = std::stoi(resString.substr(resString.find("x", 0) + 1, resString.length()).c_str());
			Resolution = res;
			CurrentSkin = Skin::AppSkin(j["Skin"]);
			Volume = j["Volume"] / 100;
			return *this;
		}

		Settings::operator nlohmann::json() const
		{
			json j;
			j["Resolution"] = std::to_string(Resolution.DisplayWidth) + "x" + std::to_string(Resolution.DisplayHeight);
			j["Fullscreen"] = Resolution.Fullscreen;
			j["FpsCap"] = Resolution.FpsCap;
			j["Skin"] = CurrentSkin.SkinPath;
			j["Volume"] = Volume * 100;
			return j;
		}

		Settings::~Settings()
		{

		}
	}
}