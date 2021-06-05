#include <OnBeat/Config/Config.h>
#define MAGIC_ENUM_RANGE_MIN -128
#define MAGIC_ENUM_RANGE_MAX 512
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
			j = json{
				{ "COLUMN_1", magic_enum::enum_name(i.COLUMN_1) },
				{ "COLUMN_2", magic_enum::enum_name(i.COLUMN_2) },
				{ "COLUMN_3", magic_enum::enum_name(i.COLUMN_3) },
				{ "COLUMN_4", magic_enum::enum_name(i.COLUMN_4) },
				{ "PAUSE"   , magic_enum::enum_name(i.PAUSE   ) },
			};
		}

		void from_json(const json& j, InputMap& i)
		{
			auto enumCast = [j](const std::string& key)
			{
				std::string jsonValue = j.value(key, OB_UNDEFINED_KEY);

				auto cast = magic_enum::enum_cast<Keys>(jsonValue);
				if (cast.has_value())
				{
					return cast.value();
				}
				else
				{
					return Keys::Undefined;
				}
			};

			i.COLUMN_1 = enumCast("COLUMN_1");
			i.COLUMN_2 = enumCast("COLUMN_2");
			i.COLUMN_3 = enumCast("COLUMN_3");
			i.COLUMN_4 = enumCast("COLUMN_4");

			i.PAUSE = enumCast("PAUSE");
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

			//Input setting
			set(newS.Input.COLUMN_1, oldS.Input.COLUMN_1);
			set(newS.Input.COLUMN_2, oldS.Input.COLUMN_2);
			set(newS.Input.COLUMN_3, oldS.Input.COLUMN_3);
			set(newS.Input.COLUMN_4, oldS.Input.COLUMN_4);
			set(newS.Input.PAUSE, oldS.Input.PAUSE);

			return true;
		}

		//Ensure that a settings struct has been made correctly
		bool validateSettings(const Settings& s, bool allowUndefined)
		{
			auto checkRange = [allowUndefined](auto& val, const auto& min, const auto& max)
			{
				if (allowUndefined)
				{
					if (val != OB_UNDEFINED_INT && (val > max || val < min))
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
			if (!checkRange(s.Resolution.FpsCap, 0, 1000)) return false;
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
			j["Input"] = s.Input;
		}

		void from_json(const json& j, Settings& s)
		{
			setResolution(s, j.value("Resolution", ""));

			s.Resolution.Fullscreen = j.value("Fullscreen", OB_UNDEFINED_INT);

			s.Resolution.FpsCap = j.value("FpsCap", OB_UNDEFINED_INT);

			s.CurrentSkin = Skin::AppSkin(j.value("Skin", OB_DEFAULT_SKIN));

			s.Volume = j.value("Volume", (float)OB_UNDEFINED_INT);

			s.Input = j.value("Input", InputMap{});
		}
	}
}