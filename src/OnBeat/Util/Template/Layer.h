#pragma once
#include <OnBeat/Util/Discord/Integration.h>
#include <Hazel/Core/Layer.h>
#include <Hazel/Events/ApplicationEvent.h>
#include <Hazel/Renderer/OrthographicCamera.h>

namespace OnBeat
{
	class Layer : public Hazel::Layer
	{
		public:
			Layer(const std::string& name = "OnBeatLayer");
			~Layer();

			void OnEvent(Hazel::Event& e) override;
			bool OnWindowResize(Hazel::WindowResizeEvent& e);

		protected:
			Hazel::Scope<Hazel::OrthographicCamera> CameraController;
			discord::Activity activity{};

			void CreateCamera(uint32_t width, uint32_t height);
	};
}