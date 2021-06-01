#include <OnBeat/Util/JS/JS.h>

namespace ultralight
{
	void to_json(nlohmann::json& j, const JSObject& obj)
	{
		JSPropertyNameArrayRef names = JSObjectCopyPropertyNames(obj.context(), obj);
		size_t len = JSPropertyNameArrayGetCount(names);
		for (int i = 0; i < len; i++)
		{
			JSString propertyName = JSPropertyNameArrayGetNameAtIndex(names, i);
			String ulString = propertyName;

			JSValueRef e = nullptr;
			JSValue val = JSObjectGetProperty(obj.context(), obj, propertyName, &e);

			if (!e)
			{
				const char* key = ulString.utf8().data();
				//j[ulString.utf8().data()] = s.utf8().data();
				if (val.IsNumber())
				{
					j[key] = val.ToNumber();
				}
				else if (val.IsNull() || val.IsUndefined())
				{
					j[key] = NULL;
				}
				else if (val.IsBoolean())
				{
					j[key] = val.ToBoolean();
				}
				else if (val.IsObject())
				{
					j[key] = val;
				}
				else
				{
					String s = val;
					j[key] = s.utf8().data();
				}
			}
		}
	}

	void from_json(const nlohmann::json& j, JSObject& obj)
	{
		obj = (JSObjectRef)JSValueMakeFromJSONString(obj.context(), JSString(j.dump().c_str()));
	}
}