#pragma once
#include <ultralight/MouseEvent.h>
#include <glm/glm.hpp>
#include <AppCore/JSHelpers.h>
#include <nlohmann/json.hpp>

typedef struct GLFWcursor GLFWcursor;

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

		bool checkPath(std::string file, bool makePath);

		int HazelKeyCodeToUl(int key);
		int UlKeyCodeToHazel(int key);

		ultralight::MouseEvent::Button HazelMouseCodeToUl(int key);

	}
}