#include <OnBeat/Ui/MainMenu/MainMenu.h>
#include <OnBeat/App/App.h>
#include <Hazel/Utils/PlatformUtils.h>
#include <glfw/glfw3.h>
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
		App::Get().GetLayerStack().SetCallback(BindPostUpdateCallback(&MainMenu::StartMusicLayer));
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

		Config::Settings newSettings;

		try
		{
			newSettings = config;
		}
		catch (const std::exception& e)
		{
			//Push error to menu
			HZ_ERROR("Error casting arguments to settings. Exception: " + std::string(e.what()));
			return;
		}

		if (!Config::validateSettings(newSettings))
		{
			HZ_WARN("UpdateSettings received an invalid settings json.");
			return;
		}

		App::Get().SetSettings(newSettings);

		App::Get().RefreshSettings();
		return;
	}

	void MainMenu::RevertSettings(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		if (obj["SetSettings"].IsFunction())
		{
			nlohmann::json jsonSettings = App::Get().GetSettings();
			ul::JSObject arg = jsonSettings;

			obj["SetSettings"].ToFunction()(ul::JSArgs{ (ul::JSValue)arg });
		}

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

	ul::JSValue MainMenu::GetHWInfo(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		int count;
		GLFWmonitor** monitors = glfwGetMonitors(&count);

		int Lwidth = 0, Lheight = 0;

		for (int i = 0; i < count; i++)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(monitors[i]);

			if (mode->width > Lwidth)
				Lwidth = mode->width;

			if (mode->height > Lheight)
				Lheight = mode->height;
		}

		ul::JSObject ret;
		ret["width"] = Lwidth;
		ret["height"] = Lheight;
		ret["monitors"] = count;
		return (ul::JSValue)ret;
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
		globalObj["GetHWInfo"] = BindJSCallbackWithRetval(&MainMenu::GetHWInfo);

		//Notify JS Content is loaded
		if (globalObj["DOMLoaded"].IsFunction())
			globalObj["DOMLoaded"].ToFunction()(ul::JSArgs());
	}

	void MainMenu::StartMusicLayer()
	{
		App::Get().StartGame("testing/testMusic/test.wav");
		return;
	}

	MainMenu::~MainMenu()
	{

	}
}