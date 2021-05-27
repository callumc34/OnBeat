#pragma once
#include <OnBeat/App/App.h>
#include <Hazel/Core/Layer.h>
#include <Hazel/Renderer/OrthographicCamera.h>

namespace OnBeat
{
	class Layer : public Hazel::Layer
	{
		public:
			Layer(std::string name = "OnBeatLayer");
			~Layer();

			void OnEvent(Hazel::Event& e) override;
			bool OnWindowResize(Hazel::WindowResizeEvent& e);

			virtual void EventUpdate(Hazel::Event& e);

		protected:
			Hazel::Scope<Hazel::OrthographicCamera> CameraController;

			void CreateCamera(uint32_t width, uint32_t height);

		private:
	};
}