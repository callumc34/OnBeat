#pragma once
#include <AppCore/JSHelpers.h>
#include <JavaScriptCore/JSRetainPtr.h>
#include <nlohmann/json.hpp>

namespace ultralight
{
	void to_json(nlohmann::json& j, const JSObject& obj);

	void from_json(const nlohmann::json& j, JSObject& obj);
}