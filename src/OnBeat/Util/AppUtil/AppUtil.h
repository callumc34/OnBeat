#pragma once
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

namespace OnBeat
{
	namespace Util
	{
		enum class Flag : uint8_t
		{
			WIDTH = 1,
			HEIGHT = 2,
		};

		glm::vec4 arrayToVec4(nlohmann::json object);

		float stringSizeToFloat(std::string val, Flag flag);
	}
}