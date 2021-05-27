#include <OnBeat/Ui/MainMenu/MainMenu.h>
#include <Hazel/Utils/PlatformUtils.h>
#include <JavaScriptCore/JSRetainPtr.h>
#include <AppCore/JSHelpers.h>
#include <algorithm>

namespace ul = ultralight;

namespace OnBeat
{
	namespace JSCallbacks
	{
		//Start game callback
		JSValueRef StartGame(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception)
		{
			
			return JSValueMakeNull(ctx);
		}

		//Exit game callback
		JSValueRef ExitGame(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception)
		{
			Hazel::Application::Get().Close();

			return JSValueMakeNull(ctx);
		}

		//Select file callback
		JSValueRef SelectSkin(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception)
		{
			auto dialog = Hazel::FileDialogs::OpenFile("All Files (.*)");
			if (dialog.has_value())
			{
				//Injection unsafe but considering you can just edit the JS file yourself it's probably fine
				std::string path = dialog.value();
				std::replace(path.begin(), path.end(), '\\', '/');
				std::string script = "Skin.value = '" + path + "';";
				JSEvaluateScript(ctx, ul::JSString(script.c_str()), NULL, NULL, 0, 0);
			}
			return JSValueMakeNull(ctx);
		}

		//Apply settings callback
		JSValueRef PushSettings(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception)
		{
			//Todo cast these to JSObject types using JSValueToObject then to JSValue using JSObjectGetProperty
			//Then cast to ctypes and check for errors if they havent been passed
			//Update the Config with the new values
			JSValueRef e = 0;

			JSObjectRef cfgObject = JSValueToObject(ctx, arguments[0], &e);
			//Todo check exception
			if (e)
			{
				//Handle error
				return nullptr;
			}

			//Todo set newSettings to current settings and overwrite
			Config::Settings newSettings = App::Get().Settings;

			//Convert all object properties to C variables
			if (JSObjectHasProperty(ctx, cfgObject, ul::JSString("Resolution")))
			{
				JSValueRef value = JSObjectGetProperty(ctx, cfgObject, ul::JSString("Resolution"), 0);
				JSStringRef str = JSValueToStringCopy(ctx, value, 0);
				ul::JSString ulJSStr = str;
				ul::String ulString = ulJSStr;
				std::string stdString = ulString.utf8().data();
				newSettings.DisplayWidth = std::stoi(stdString.substr(0, stdString.find("x", 0)).c_str());
				newSettings.DisplayHeight = std::stoi(stdString.substr(stdString.find("x", 0) + 1, stdString.length()).c_str());
				JSStringRelease(str);
			}
			if (JSObjectHasProperty(ctx, cfgObject, ul::JSString("Fullscreen")))
			{
				JSValueRef value = JSObjectGetProperty(ctx, cfgObject, ul::JSString("Fullscreen"), 0);
				JSStringRef str = JSValueToStringCopy(ctx, value, 0);
				ul::JSString ulJSStr = str;
				ul::String ulString = ulJSStr;
				newSettings.Fullscreen = std::strtol(ulString.utf8().data(), nullptr, 10);
				JSStringRelease(str);
			}
			if (JSObjectHasProperty(ctx, cfgObject, ul::JSString("FpsCap")))
			{
				JSValueRef value = JSObjectGetProperty(ctx, cfgObject, ul::JSString("FpsCap"), 0);
				JSStringRef str = JSValueToStringCopy(ctx, value, 0);
				ul::JSString ulJSStr = str;
				ul::String ulString = ulJSStr;
				newSettings.FpsCap = std::strtod(ulString.utf8().data(), nullptr);
				JSStringRelease(str);
			}
			if (JSObjectHasProperty(ctx, cfgObject, ul::JSString("Skin")))
			{
				JSValueRef value = JSObjectGetProperty(ctx, cfgObject, ul::JSString("Skin"), 0);
				JSStringRef str = JSValueToStringCopy(ctx, value, 0);
				ul::JSString ulJSStr = str;
				ul::String ulString = ulJSStr;
				newSettings.CurrentSkinPath = std::string(ulString.utf8().data());
				newSettings.CurrentSkin = Skin::AppSkin(newSettings.CurrentSkinPath.c_str());

				//Load the new menu document
				Hazel::Application::Get().PushLayer(
					new MainMenu(newSettings.CurrentSkinPath + "/menu/MainMenu/main.html", "MainMenu"));

				JSStringRelease(str);
			}
			if (JSObjectHasProperty(ctx, cfgObject, ul::JSString("LeftColumnKey")))
			{
				JSValueRef value = JSObjectGetProperty(ctx, cfgObject, ul::JSString("LeftColumnKey"), 0);
				JSStringRef str = JSValueToStringCopy(ctx, value, 0);
				ul::JSString ulJSStr = str;
				ul::String ulString = ulJSStr;
				JSStringRelease(str);
			}
			if (JSObjectHasProperty(ctx, cfgObject, ul::JSString("Volume")))
			{
				//Complete mess - may need to clean up but JSHelpers aren't working
				JSValueRef value = JSObjectGetProperty(ctx, cfgObject, ul::JSString("Volume"), 0);
				JSStringRef str = JSValueToStringCopy(ctx, value, 0);
				ul::JSString ulJSStr = str;
				ul::String ulString = ulJSStr;
				newSettings.Volume = std::strtod(ulString.utf8().data(), nullptr) / 100;
				JSStringRelease(str);
			}

			App::Get().Settings = newSettings;
			
			return JSValueMakeNull(ctx);
		}

		//Cancel settings callback
		JSValueRef RevertSettings(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception)
		{
			//Create a JSON string and pass it to the JS function
			nlohmann::json jsonSettings = App::Get().Settings;
			std::string jsonString = jsonSettings.dump();

			HZ_INFO(jsonString);
			
			//Call the JS function to set settings
			JSRetainPtr<JSStringRef> str = adopt(JSStringCreateWithUTF8CString("setSettings"));
			JSValueRef func = JSEvaluateScript(ctx, str.get(), 0, 0, 0, 0);

			if (JSValueIsObject(ctx, func))
			{
				JSObjectRef funcObj = JSValueToObject(ctx, func, 0);

				if (funcObj && JSObjectIsFunction(ctx, funcObj))
				{
					JSRetainPtr<JSStringRef> jsonArg = adopt(JSStringCreateWithUTF8CString(jsonString.c_str()));
					const JSValueRef args[] = { JSValueMakeString(ctx, jsonArg.get()) };

					size_t num_args = sizeof(args) / sizeof(JSValueRef*);

					JSValueRef exception = 0;

					JSValueRef result = JSObjectCallAsFunction(ctx, funcObj, 0, num_args, args, &exception);

					if (exception)
					{
						//Handle error
					}
				}
			}

			return JSValueMakeNull(ctx);
		}
	}

	MainMenu::MainMenu(const std::string& document, std::string layerName)
		: Menu(document, layerName)
	{
	}

	void MainMenu::UpdateMenu(Hazel::Timestep ms)
	{
		return;
	}

	void MainMenu::OnDOMReady(ultralight::View* caller, uint64_t frame_id,
		bool is_main_frame, const ultralight::String& url)
	{
		//Create JS Callbacks
		ul::Ref<ul::JSContext>context = caller->LockJSContext();
		JSContextRef ctx = context.get();
		JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

		//Start game callback
		JSStringRef StartGameName = JSStringCreateWithUTF8CString("StartGame");
		JSObjectRef SGfunc = JSObjectMakeFunctionWithCallback(ctx, StartGameName,
			JSCallbacks::StartGame);
		JSObjectSetProperty(ctx, globalObj, StartGameName, SGfunc, 0, 0);
		JSStringRelease(StartGameName);

		//Exit game callback
		JSStringRef ExitGameName = JSStringCreateWithUTF8CString("ExitGame");
		JSObjectRef EGfunc = JSObjectMakeFunctionWithCallback(ctx, ExitGameName,
			JSCallbacks::ExitGame);
		JSObjectSetProperty(ctx, globalObj, ExitGameName, EGfunc, 0, 0);
		JSStringRelease(ExitGameName);

		//Apply settings callback
		JSStringRef PushSettingsName = JSStringCreateWithUTF8CString("PushSettings");
		JSObjectRef PSfunc = JSObjectMakeFunctionWithCallback(ctx, PushSettingsName,
			JSCallbacks::PushSettings);
		JSObjectSetProperty(ctx, globalObj, PushSettingsName, PSfunc, 0, 0);
		JSStringRelease(PushSettingsName);

		//Revert settings callback
		JSStringRef RevertSettingsName = JSStringCreateWithUTF8CString("RevertSettings");
		JSObjectRef RSfunc = JSObjectMakeFunctionWithCallback(ctx, RevertSettingsName,
			JSCallbacks::RevertSettings);
		JSObjectSetProperty(ctx, globalObj, RevertSettingsName, RSfunc, 0, 0);
		JSStringRelease(RevertSettingsName);

		//Select skin callback
		JSStringRef SelectSkinName = JSStringCreateWithUTF8CString("SelectSkin");
		JSObjectRef SSfunc = JSObjectMakeFunctionWithCallback(ctx, SelectSkinName,
			JSCallbacks::SelectSkin);
		JSObjectSetProperty(ctx, globalObj, SelectSkinName, SSfunc, 0, 0);
		JSStringRelease(SelectSkinName);
	}

	MainMenu::~MainMenu()
	{

	}
}