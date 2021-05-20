#include <OnBeat/Ui/MainMenu/MainMenu.h>
#include <AppCore/JSHelpers.h>

namespace ul = ultralight;

namespace OnBeat
{
	namespace JSCallbacks
	{
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

			return JSValueMakeNull(ctx);
		}

		//Apply settings callback
		JSValueRef PushSettings(JSContextRef ctx, JSObjectRef function,
			JSObjectRef thisObject, size_t argumentCount,
			const JSValueRef arguments[], JSValueRef* exception)
		{
			//Todo cast these to JSObject types using JSValueToObject then to JSValue using JSObjectGetProperty
			//Then cast to ctypes and check for errors if they havent been passed
			//Update the MainApp->Config with the new values
			JSValueRef e = nullptr;

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
			return JSValueMakeNull(ctx);
		}
	}

	MainMenu::MainMenu(const std::string& document, App* MainApp, std::string layerName)
		: Menu(document, MainApp, layerName)
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