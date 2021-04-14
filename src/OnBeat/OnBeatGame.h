#include <Hazel.h>

class OnBeatGame : public Hazel::Layer
{
	public:
		OnBeatGame();

		//OnEvents
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnEvent(Hazel::Event& e) override;
		void OnUpdate(Hazel::Timestep ts) override;

		virtual void OnImGuiRender() override;

	private:
		Hazel::OrthographicCameraController cameraController;


};