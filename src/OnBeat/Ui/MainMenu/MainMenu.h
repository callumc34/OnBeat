#pragma once
#include <OnBeat/Util/Template/Menu.h>
#include <OnBeat/Util/JS/JS.h>

namespace OnBeat
{
	class MainMenu : public Menu
	{
		public:
			MainMenu(const std::string& document, std::string layerName);
			~MainMenu();

			//JS Callbacks to C
			void StartGame(const ultralight::JSObject& obj, const ultralight::JSArgs& args);
			void ExitGame(const ultralight::JSObject& obj, const ultralight::JSArgs& args);
			void UpdateSettings(const ultralight::JSObject& obj, const ultralight::JSArgs& args);
			void RevertSettings(const ultralight::JSObject& obj, const ultralight::JSArgs& args);
			ultralight::JSValue SelectSkin(const ultralight::JSObject& obj, const ultralight::JSArgs& args);

			void UpdateMenu(Hazel::Timestep ms) override;

			void OnDOMReady(ultralight::View* caller,
				uint64_t frame_id,
				bool is_main_frame,
				const ultralight::String& url) override;

		private:

	};
}