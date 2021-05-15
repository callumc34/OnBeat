#include <OnBeat/Ui/MainMenu/MainMenu.h>

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

		//Exit game callbacks
		JSStringRef ExitGameName = JSStringCreateWithUTF8CString("ExitGame");
		JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, ExitGameName,
			JSCallbacks::ExitGame);
		JSObjectRef globalObj = JSContextGetGlobalObject(ctx);
		JSObjectSetProperty(ctx, globalObj, ExitGameName, func, 0, 0);
		JSStringRelease(ExitGameName);

	}

	MainMenu::~MainMenu()
	{

	}
}