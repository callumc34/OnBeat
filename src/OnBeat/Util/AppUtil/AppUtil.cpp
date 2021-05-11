#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <Hazel/Core/Application.h>

namespace OnBeat
{
	namespace Util
	{

		glm::vec4 arrayToVec4(nlohmann::json object)
		{
			return glm::vec4(object[0], object[1], object[2], object[3]);
		}

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
	}
}