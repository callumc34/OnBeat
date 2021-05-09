#pragma once
#include <Ultralight/Ultralight.h>
#include <OnBeat/Ui/OpenGLSurface.h>
#include <Hazel.h>

class OnBeatMenu : public Hazel::Layer
{
	public:
		OnBeatMenu(const std::string& document);

		void LoadDocument(const ultralight::String& document);

		void OnEvent(Hazel::Event& e) override;
		void OnUpdate(Hazel::Timestep ms) override;
		bool OnWindowResize(Hazel::WindowResizeEvent& ms);

	private:
		void CreateCamera(uint32_t width, uint32_t height);

		Hazel::Scope<Hazel::OrthographicCamera> cameraController;

		ultralight::Config config;
		ultralight::RefPtr<ultralight::Renderer> renderer;
		ultralight::RefPtr<ultralight::View> view;
		std::unique_ptr<GLTextureSurfaceFactory> factory;

		std::string documentPath;

};