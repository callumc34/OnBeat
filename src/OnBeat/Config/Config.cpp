#include <OnBeat/Config/Config.h>
#define MAGIC_ENUM_RANGE_MIN -128
#define MAGIC_ENUM_RANGE_MAX 512
#include <magic_enum.hpp>
#include <filesystem>
#include <fstream>
#include <algorithm>

//Macros to shorten default expressions
#define DEFAULT_SET(var) j[#var] = c.var
#define DEFAULT_GET(var) c.var = j.value(#var, OB_UNDEFINED_INT)
#define DEFAULT_SWAP(var) set(newS.var, oldS.var);
#define DEFAULT_VALIDATE(var, min, max) if (!checkRange(c.var, min, max)) return false;

using json = nlohmann::json;

namespace OnBeat
{
	namespace Config
	{
		//Set resolution from a json resolution string
		void setResolution(VideoConfig& c, const std::string& resString)
		{
			try
			{
				c.DisplayWidth = std::stoi(resString.substr(0, resString.find("x", 0)).c_str());
				c.DisplayHeight = std::stoi(resString.substr(resString.find("x", 0) + 1, resString.length()).c_str());
			}
			catch (const std::exception& e)
			{
				HZ_WARN("Invalid resolution string: " + std::string(e.what()));
				c.DisplayWidth = OB_UNDEFINED_INT;
				c.DisplayHeight = OB_UNDEFINED_INT;
			}
		}

		void to_json(json& j, const VideoConfig& c)
		{
			j["Resolution"] = std::to_string(c.DisplayWidth) + "x" + std::to_string(c.DisplayHeight);
			DEFAULT_SET(Fullscreen);
			DEFAULT_SET(VSync);
			DEFAULT_SET(FpsCap);
		}

		void from_json(const json& j, VideoConfig& c)
		{
			setResolution(c, j.value("Resolution", ""));
			DEFAULT_GET(Fullscreen);
			DEFAULT_GET(FpsCap);
			DEFAULT_GET(VSync);
		}

		void to_json(json& j, const InputConfig& c)
		{
			j = json{
				{ "COLUMN_1", magic_enum::enum_name(c.COLUMN_1) },
				{ "COLUMN_2", magic_enum::enum_name(c.COLUMN_2) },
				{ "COLUMN_3", magic_enum::enum_name(c.COLUMN_3) },
				{ "COLUMN_4", magic_enum::enum_name(c.COLUMN_4) },
				{ "PAUSE"   , magic_enum::enum_name(c.PAUSE) },
			};
		}

		void from_json(const json& j, InputConfig& c)
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

			c.COLUMN_1 = enumCast("COLUMN_1");
			c.COLUMN_2 = enumCast("COLUMN_2");
			c.COLUMN_3 = enumCast("COLUMN_3");
			c.COLUMN_4 = enumCast("COLUMN_4");
	
			c.PAUSE = enumCast("PAUSE");
		}

		void to_json(json& j, const AudioConfig& c)
		{
			DEFAULT_SET(Volume);
		}

		void from_json(const json& j, AudioConfig& c)
		{
			c.Volume = j.value("Volume", (float)OB_UNDEFINED_INT);
		}

		void to_json(json& j, const GameConfig& c)
		{
			DEFAULT_SET(CameraVelocity);
			DEFAULT_SET(ThresholdConstant);
			DEFAULT_SET(ThresholdMultiple);
			DEFAULT_SET(MeanWindow);
			DEFAULT_SET(MaximaWindow);
			j["Skin"] = c.Skin.SkinPath;
		}

		void from_json(const json& j, GameConfig& c)
		{
			DEFAULT_GET(CameraVelocity);
			DEFAULT_GET(MeanWindow);
			DEFAULT_GET(MaximaWindow);
			c.ThresholdConstant = j.value("ThresholdConstant", (double)OB_UNDEFINED_INT);
			c.ThresholdMultiple = j.value("ThresholdMultiple", (double)OB_UNDEFINED_INT);
			c.Skin = Skin::AppSkin(j.value("Skin", OB_DEFAULT_SKIN));
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

			//Video config
			DEFAULT_SWAP(Video.DisplayWidth);
			DEFAULT_SWAP(Video.DisplayHeight);
			DEFAULT_SWAP(Video.VSync);
			DEFAULT_SWAP(Video.FpsCap);
			DEFAULT_SWAP(Video.Fullscreen);

			//Input config
			DEFAULT_SWAP(Input.COLUMN_1);
			DEFAULT_SWAP(Input.COLUMN_2);
			DEFAULT_SWAP(Input.COLUMN_3);
			DEFAULT_SWAP(Input.COLUMN_4);
			DEFAULT_SWAP(Input.PAUSE);

			//Audio config
			DEFAULT_SWAP(Audio.Volume);

			//Game config
			DEFAULT_SWAP(Game.CameraVelocity);
			DEFAULT_SWAP(Game.ThresholdConstant);
			DEFAULT_SWAP(Game.ThresholdMultiple);
			DEFAULT_SWAP(Game.MeanWindow);
			DEFAULT_SWAP(Game.MaximaWindow);

			return true;
		}

		//Ensure that a settings struct has been made correctly
		bool validateSettings(const Settings& c, bool allowUndefined)
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

			//Video config
			DEFAULT_VALIDATE(Video.DisplayWidth, 64, 16000);
			DEFAULT_VALIDATE(Video.DisplayHeight, 64, 16000);
			DEFAULT_VALIDATE(Video.VSync, 0, 8);
			DEFAULT_VALIDATE(Video.FpsCap, 0, 3000);
			DEFAULT_VALIDATE(Video.Fullscreen, -1, 64);

			//Audio config
			DEFAULT_VALIDATE(Audio.Volume, 0, 1);

			//Game config
			DEFAULT_VALIDATE(Game.CameraVelocity, 0, 100);
			DEFAULT_VALIDATE(Game.ThresholdConstant, 0, 1);
			DEFAULT_VALIDATE(Game.ThresholdMultiple, 0, 1);
			DEFAULT_VALIDATE(Game.MeanWindow, 3, 32);
			DEFAULT_VALIDATE(Game.MaximaWindow, 3, 32);

			return true;
		}

		//Cast settings to and from json
		void to_json(json& j, const Settings& c)
		{
			DEFAULT_SET(Video);
			DEFAULT_SET(Input);
			DEFAULT_SET(Audio);
			DEFAULT_SET(Game);
		}

		void from_json(const json& j, Settings& c)
		{
			c.Video = j["Video"];
			c.Input = j["Input"];
			c.Audio = j["Audio"];
			c.Game = j["Game"];
		}
	}
}