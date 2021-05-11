#pragma once
#include <OnBeat/Util/Template/Menu.h>

namespace OnBeat
{
	class PauseMenu : public Menu
	{
		public:
			PauseMenu(const std::string& document, App* MainApp, std::string layerName);
			~PauseMenu();
	};
}