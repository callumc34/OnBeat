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
		//Cast input struct to and from json
		void to_json(json& j, const InputMap& i)
		{

		}

		void from_json(const json& j, InputMap& i)
		{

		}

		//Setup onbeat settings from json file
		Settings Settings::Create(const std::string& path)
		{
			std::ifstream input(path);
			json config;
			input >> config;
			input.close();

			return config;
		}

		bool swapSettings(Settings& newS, const Settings& oldS)
		{
			if (!validateSettings(newS) && !validateSettings(oldS))
				return false;

			//Quick define swapping variables
			auto set = [](auto& newS, const auto& oldS)
			{
				if (oldS != OB_UNDEFINED_INT)
					newS = oldS;
			};

			//Quick set values that can be defined by an int
			set(newS.Resolution.DisplayWidth, oldS.Resolution.DisplayWidth);
			set(newS.Resolution.DisplayHeight, oldS.Resolution.DisplayHeight);
			set(newS.Resolution.Fullscreen, oldS.Resolution.Fullscreen);
			set(newS.Resolution.FpsCap, oldS.Resolution.FpsCap);
			set(newS.Volume, oldS.Volume);

			return true;
		}

		//Ensure that a settings struct has been made correctly
		bool validateSettings(const Settings& s, bool allowUndefined)
		{
			auto checkRange = [allowUndefined](auto& val, const auto& min, const auto& max)
			{
				if (allowUndefined)
				{
					if (val == OB_UNDEFINED_INT)
					{
						return true;
					}
					else if ((val > max || val < min))
					{
						return false;
					}
				}
				else
				{
					if (val == OB_UNDEFINED_INT || (val > max || val < min))
					{
						return false;
					}
				}
				return true;
			};

			if (!checkRange(s.Resolution.DisplayWidth, 64, 16000)) return false;
			if (!checkRange(s.Resolution.DisplayHeight, 64, 16000)) return false;
			if (!checkRange(s.Resolution.Fullscreen, -1, 64)) return false;
			if (!checkRange(s.Resolution.FpsCap, 0, 64)) return false;
			if (!checkRange(s.Volume, 0, 1)) return false;
			return true;
		}

		//Set resolution from a json resolution string
		void setResolution(Settings& s, const std::string& resString)
		{
			try
			{
				s.Resolution.DisplayWidth = std::stoi(resString.substr(0, resString.find("x", 0)).c_str());
				s.Resolution.DisplayHeight = std::stoi(resString.substr(resString.find("x", 0) + 1, resString.length()).c_str());
			}
			catch (const std::exception& e)
			{
				s.Resolution.DisplayWidth == OB_UNDEFINED_INT;
				s.Resolution.DisplayHeight == OB_UNDEFINED_INT;
			}
		}


		//Cast settings to and from json
		void to_json(json& j, const Settings& s)
		{
			j["Resolution"] = std::to_string(s.Resolution.DisplayWidth) + "x" + std::to_string(s.Resolution.DisplayHeight);
			j["Fullscreen"] = s.Resolution.Fullscreen;
			j["FpsCap"] = s.Resolution.FpsCap;
			j["Skin"] = s.CurrentSkin.SkinPath;
			j["Volume"] = s.Volume;
		}

		void from_json(const json& j, Settings& s)
		{
			setResolution(s, j.value("Resolution", ""));

			s.Resolution.Fullscreen = j.value("Fullscreen", OB_UNDEFINED_INT);

			s.Resolution.FpsCap = j.value("FpsCap", OB_UNDEFINED_INT);

			s.CurrentSkin = Skin::AppSkin(j.value("Skin", OB_DEFAULT_SKIN));

			s.Volume = j.value("Volume", OB_UNDEFINED_INT);
		}
	}
}