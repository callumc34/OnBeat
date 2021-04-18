#pragma once
#include <Hazel.h>
#include <OnBeat/OnBeat.h>

class MusicLayer : public Hazel::Layer
{
	public:
		MusicLayer(OnBeat* app);

		//OnEvents
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnEvent(Hazel::Event& e) override;
		void OnUpdate(Hazel::Timestep ts) override;
		bool OnWindowResize(Hazel::WindowResizeEvent& e);

		virtual void OnImGuiRender() override;

	private:
		void CreateCamera(uint32_t width, uint32_t height);

		Hazel::Scope<Hazel::OrthographicCamera> cameraController;
		OnBeat* MainApp;


};