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

	void MainMenu::ExitGame(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		Hazel::Application::Get().Close();
		return;
	}

	void MainMenu::UpdateSettings(const ul::JSObject& obj, const ul::JSArgs& args)
	{
		Config::Settings newSettings = App::Get().Settings;

		//Convert all object properties to C variables
		Config::ResolutionData newResolution = newSettings.getResolution();
		if (obj.HasProperty("Resolution"))
		{
			//Cast to ultralight string
			ul::String ulString = obj["Resolution"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newResolution.DisplayWidth = std::stoi(stdString.substr(0, stdString.find("x", 0)).c_str());
				newResolution.DisplayHeight = std::stoi(stdString.substr(stdString.find("x", 0) + 1, stdString.length()).c_str());
			}
		}
		if (obj.HasProperty("Fullscreen"))
		{
			ul::String ulString = obj["Fullscreen"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newResolution.Fullscreen = std::strtol(stdString.c_str(), nullptr, 10);
			}
		}
		if (obj.HasProperty("FpsCap"))
		{
			ul::String ulString = obj["FpsCap"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				newResolution.FpsCap = std::strtod(stdString.c_str(), nullptr);
			}
		}
		//Set new resolution
		newSettings.setResolution(newResolution);

		if (obj.HasProperty("Skin"))
		{
			ul::String ulString = obj["Skin"].ToObject()["value"].ToString();
			std::string stdString = ulString.utf8().data();
			if (stdString.length() > 0)
			{
				try
				{
					newSettings.setCurrentSkin(Skin::AppSkin(stdString));
					LoadDocument((newSettings.getCurrentSkin().SkinDirectory + "/menu/MainMenu/main.html").c_str());
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
				newSettings.setVolume(std::strtod(stdString.c_str(), nullptr) / 100);
			}
		}

		App::Get().Settings = newSettings;
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