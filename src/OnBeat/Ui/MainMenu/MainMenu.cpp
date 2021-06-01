#include <OnBeat/Ui/MainMenu/MainMenu.h>
#include <Hazel/Utils/PlatformUtils.h>
#include <algorithm>

namespace ul = ultralight;

namespace OnBeat
{
	MainMenu::MainMenu(const std::string& document, std::string layerName)
		: Menu(document, layerName)
	{
	}

	//C callbacks from JS

	void MainMenu::UpdateMenu(Hazel::Timestep ms)
	{
		return;
	}

	void MainMenu::StartGame(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		
		return;
	}

	void MainMenu::ExitGame(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		Hazel::Application::Get().Close();
		return;
	}

	void MainMenu::UpdateSettings(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		if (args.size() != 1)
		{
			//Push error to menu
			HZ_WARN("UpdateSettings recieved invalid args of size: " + args.size());
			return;
		}
		nlohmann::json config = args[0];

		Config::Settings newSettings = config;

		try
		{
			newSettings = config;
		}
		catch (const std::exception& e)
		{
			//Push error to menu
			HZ_ERROR("Error casting arguments to settings.");
			return;
		}

		if (!Config::validateSettings(newSettings))
		{
			HZ_WARN("UpdateSettings received an invalid settings json.");
			return;
		}

		Config::swapSettings(App::Get().Settings, newSettings);

		App::Get().RefreshSettings();
		return;
	}

	void MainMenu::RevertSettings(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		nlohmann::json jsonSettings = App::Get().Settings;
		std::string script = "setSettings(" + jsonSettings.dump() + ");";
		
		view->EvaluateScript(script.c_str());

		return;
	}

	ul::JSValue MainMenu::SelectSkin(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		auto dialog = Hazel::FileDialogs::OpenFile("JSON Skin (.json)");
		if (dialog.has_value())
		{
			//Injection unsafe but considering you can just edit the JS file yourself it's probably fine
			std::string path = dialog.value();
			std::replace(path.begin(), path.end(), '\\', '/');
			return ul::JSValue(path.c_str());
		}
		return ul::JSValue(false);
	}

	void MainMenu::OnDOMReady(ul::View* caller, uint64_t frame_id,
		bool is_main_frame, const ul::String& url)
	{
		//Create JS Callbacks
		ul::Ref<ul::JSContext>context = caller->LockJSContext();
		ul::SetJSContext(context.get());
		//JSContextRef ctx = context.get();
		ul::JSObject globalObj = ul::JSGlobalObject();

		using namespace ul; //Prevent BindJSCallback error

		//C callbacks from JS
		globalObj["StartGame"] = BindJSCallback(&MainMenu::StartGame);
		globalObj["ExitGame"] = BindJSCallback(&MainMenu::ExitGame);
		globalObj["UpdateSettings"] = BindJSCallback(&MainMenu::UpdateSettings);
		globalObj["RevertSettings"] = BindJSCallback(&MainMenu::RevertSettings);
		globalObj["SelectSkin"] = BindJSCallbackWithRetval(&MainMenu::SelectSkin);
		RevertSettings(globalObj, ul::JSArgs());
	}

	MainMenu::~MainMenu()
	{

	}
}