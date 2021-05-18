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
			JSValueRef e = nullptr;
			ul::JSObject cfgObject = JSValueToObject(ctx, arguments[0], &e);
			//Todo check exception
			if (!e)
			{
				//Handle error
			}

			//Todo set newSettings to current settings and overwrite
			Config::Settings newSettings;

			//Convert all object properties to C variables
			if (cfgObject.HasProperty("Resolution") && cfgObject["Resolution"].IsString())
			{
				ul::String resString = cfgObject["Resolution"].ToString();								
			}
			if (cfgObject.HasProperty("Fullscreen") && cfgObject["Fullscreen"].IsString())
			{
				int monitor = cfgObject["Resolution"].ToInteger();
				newSettings.Fullscreen = monitor;
			}
			if (cfgObject.HasProperty("FpsCap") && cfgObject["FpsCap"].IsString())
			{
				newSettings.FpsCap = cfgObject["FpsCap"].ToInteger();
			}
			if (cfgObject.HasProperty("Skin") && cfgObject["Skin"].IsString())
			{
				ul::String skinPath = cfgObject["Skin"].ToString();
			}
			if (cfgObject.HasProperty("Volume") && cfgObject["Volume"].IsString())
			{
				newSettings.Volume = cfgObject["Volume"].ToInteger();
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
			//Update the MainApp->Config with the new values
			const JSObjectRef cfgObject = JSValueToObject(ctx, arguments[0], 0);
			
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