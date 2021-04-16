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

		virtual void OnImGuiRender() override;

	private:
		Hazel::OrthographicCameraController cameraController;
		OnBeat* MainApp;


};