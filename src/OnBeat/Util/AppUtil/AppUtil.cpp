#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <Hazel/Core/Application.h>
#include <filesystem>

namespace OnBeat
{
	namespace Util
	{
		//Convert first 4 values in a json object to a vec4
		glm::vec4 arrayToVec4(nlohmann::json object)
		{
			return glm::vec4(object[0], object[1], object[2], object[3]);
		}

		//Convert string to Hazel float value
		float stringSizeToFloat(std::string val, Flag flag)
		{
			std::string type = val.substr(val.size() - 1, val.size());
			float n = std::atof(val.c_str());
			if (type == "%")
			{
				float scale = 1600.0f;
				if (flag == Flag::HEIGHT)
				{
					scale = Hazel::Application::Get().GetWindow().GetHeight();
				}
				if (flag == Flag::WIDTH)
				{
					scale = Hazel::Application::Get().GetWindow().GetWidth();
				}
				return n * scale / 10000;
			}
			else if (type == "px")
			{
				float scale;
				if (flag == Flag::HEIGHT)
				{
					scale = Hazel::Application::Get().GetWindow().GetHeight();
				}
				if (flag == Flag::WIDTH)
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

		//Check for file and create paths if makePath
		bool checkPath(std::string file, bool makePath)
		{
			if (!std::filesystem::exists(file))
			{
				if (makePath)
				{
					std::filesystem::create_directories(
						std::filesystem::path(file).parent_path());
				}

				return false;
			}

			return true;
		}
	}
}