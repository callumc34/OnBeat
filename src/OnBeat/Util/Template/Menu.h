#pragma once
#include <OnBeat/Util/Template/GLTextureSurface.h>
#include <OnBeat/Util/Template/Layer.h>
#include <Ultralight/Ultralight.h>

namespace OnBeat
{
	class Menu : public Layer
	{
		public:
			Menu(const std::string& document, App* MainApp, std::string layerName);
			~Menu();

			void LoadDocument(const ultralight::String& document);

			void OnUpdate(Hazel::Timestep ms) override;
			void OnEvent(Hazel::Event& e) override;
			bool OnMouseMove(Hazel::MouseMovedEvent& e);
			bool ResizeView(Hazel::WindowResizeEvent& e);

			virtual void UpdateMenu(Hazel::Timestep ms) = 0;

		protected:
			std::string DocumentPath;

		private:
			//Ultralight setup
			ultralight::Config Config;
			ultralight::RefPtr<ultralight::Renderer> renderer;
			ultralight::RefPtr<ultralight::View> view;
			std::unique_ptr<GLTextureSurfaceFactory> factory;
	};
}