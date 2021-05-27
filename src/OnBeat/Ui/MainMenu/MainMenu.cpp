#include <OnBeat/Ui/MainMenu/MainMenu.h>
#include <Hazel/Utils/PlatformUtils.h>
#include <JavaScriptCore/JSRetainPtr.h>
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

	void MainMenu::ExitGame(const ultralight::JSObject& obj, const ultralight::JSArgs& args)
	{
		Hazel::Application::Get().Close();
		return;
	}

	void MainMenu::UpdateSettings(const ultralight::JSObject& obj, const ultralight::JSArgs& args)
	{
		Config::Settings newSettings = App::Get().Settings;

		//Convert all object properties to C variables
		if (obj.HasProperty("Resolution"))
		{
			//Cast to ultralight string
			ul::String ulString = obj["Resolution"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newSettings.DisplayWidth = std::stoi(stdString.substr(0, stdString.find("x", 0)).c_str());
				newSettings.DisplayHeight = std::stoi(stdString.substr(stdString.find("x", 0) + 1, stdString.length()).c_str());
			}
		}
		if (obj.HasProperty("Fullscreen"))
		{
			ul::String ulString = obj["Fullscreen"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newSettings.Fullscreen = std::strtol(stdString.c_str(), nullptr, 10);
			}
		}
		if (obj.HasProperty("FpsCap"))
		{
			ul::String ulString = obj["FpsCap"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newSettings.FpsCap = std::strtod(stdString.c_str(), nullptr);
			}
		}
		if (obj.HasProperty("Skin"))
		{
			ul::String ulString = obj["Skin"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				try
				{
					newSettings.CurrentSkin = Skin::AppSkin(stdString);
					LoadDocument((newSettings.CurrentSkin.SkinDirectory + "/menu/MainMenu/main.html").c_str());
				}
				catch (std::exception& e)
				{
					//Display error
				}

				//Load the new menu document
			}
		}
		if (obj.HasProperty("Volume"))
		{
			//Complete mess - may need to clean up but JSHelpers aren't working
			ul::String ulString = obj["Volume"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newSettings.Volume = std::strtod(stdString.c_str(), nullptr) / 100;
				App::Get().AudioPlayer.setVolume(newSettings.Volume);
			}
		}

		App::Get().Settings = newSettings;

		App::Get().SetWindowState(newSettings.Fullscreen - 1);
		return;
	}

	void MainMenu::RevertSettings(const ultralight::JSObject& obj, const ultralight::JSArgs& args)
	{
		nlohmann::json jsonSettings = App::Get().Settings;
		std::string script = "setSettings(" + jsonSettings.dump() + ");";
		
		view->EvaluateScript(script.c_str());

		return;
	}

	ultralight::JSValue MainMenu::SelectSkin(const ultralight::JSObject& obj, const ultralight::JSArgs& args)
	{
		auto dialog = Hazel::FileDialogs::OpenFile("JSON Skin (.json)");
		if (dialog.has_value())
		{
			//Injection unsafe but considering you can just edit the JS file yourself it's probably fine
			std::string path = dialog.value();
			std::replace(path.begin(), path.end(), '\\', '/');
			return ultralight::JSValue(path.c_str());
		}
		return ultralight::JSValue(false);
	}

	void MainMenu::OnDOMReady(ultralight::View* caller, uint64_t frame_id,
		bool is_main_frame, const ultralight::String& url)
	{
		//Create JS Callbacks
		ul::Ref<ul::JSContext>context = caller->LockJSContext();
		ul::SetJSContext(context.get());
		//JSContextRef ctx = context.get();
		ul::JSObject globalObj = ul::JSGlobalObject();

		using namespace ultralight; //Prevent BindJSCallback error

		//C callbacks from JS
		globalObj["StartGame"] = BindJSCallback(&MainMenu::StartGame);
		globalObj["ExitGame"] = BindJSCallback(&MainMenu::ExitGame);
		globalObj["UpdateSettings"] = BindJSCallback(&MainMenu::UpdateSettings);
		globalObj["RevertSettings"] = BindJSCallback(&MainMenu::RevertSettings);
		globalObj["SelectSkin"] = BindJSCallbackWithRetval(&MainMenu::SelectSkin);
	}

	MainMenu::~MainMenu()
	{

	}
}