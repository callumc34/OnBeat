#include <OnBeat/Util/JS/JS.h>

namespace ultralight
{
	void to_json(nlohmann::json& j, const JSObject& obj)
	{
		JSString jsonString = JSValueCreateJSONString(obj.context(), obj, 0, 0);
		String ulString = jsonString;

		j = nlohmann::json::parse(ulString.utf8().data());
	}

	void from_json(const nlohmann::json& j, JSObject& obj)
	{
		obj = (JSObjectRef)JSValueMakeFromJSONString(obj.context(), JSString(j.dump().c_str()));
	}
}