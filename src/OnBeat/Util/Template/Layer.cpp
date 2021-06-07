#include <OnBeat/Util/Template/Layer.h>
#include <OnBeat/App/App.h>

namespace OnBeat
{
	Layer::Layer(std::string name)
		: Hazel::Layer(name)
	{
		auto& window = Hazel::Application::Get().GetWindow();
		CreateCamera(window.GetWidth(), window.GetHeight());
	}

	void Layer::OnEvent(Hazel::Event& e)
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(Layer::OnWindowResize));
		EventUpdate(e);
	}

	bool Layer::OnWindowResize(Hazel::WindowResizeEvent& e)
	{
		CreateCamera(e.GetWidth(), e.GetHeight());
		return true;
	}

	void Layer::CreateCamera(uint32_t width, uint32_t height)
	{
		float aspectRatio = (float)width / (float)height;

		float camHeight = (float)height / 200;
		float bottom = -camHeight;
		float top = camHeight;
		float left = bottom * aspectRatio;
		float right = top * aspectRatio;
		CameraController = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
	}

	void Layer::EventUpdate(Hazel::Event& e)
	{

	}

	Layer::~Layer()
	{
	}
}