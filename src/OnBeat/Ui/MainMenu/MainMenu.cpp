#include <OnBeat/Ui/MainMenu/MainMenu.h>

namespace OnBeat
{
	MainMenu::MainMenu(const std::string& document, App* MainApp, std::string layerName)
		: Menu(document, MainApp, layerName)
	{
	}

	void MainMenu::UpdateMenu(Hazel::Timestep ms)
	{
		return;
	}

	MainMenu::~MainMenu()
	{

	}
}