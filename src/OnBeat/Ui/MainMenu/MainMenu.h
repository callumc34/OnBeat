#pragma once
#include <OnBeat/Util/Template/Menu.h>

namespace OnBeat
{
	class MainMenu : public Menu
	{
		public:
			MainMenu(const std::string& document, App* MainApp, std::string layerName);
			~MainMenu();

			void UpdateMenu(Hazel::Timestep ms) override;

		private:

	};
}