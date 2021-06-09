#include <OnBeat/App/LayerStack/LayerStack.h>

namespace OnBeat {

	LayerStack::LayerStack() : Layer("Layer Stack")
	{

	}
	
	void LayerStack::OnUpdate(Hazel::Timestep ts)
	{
		for (auto it = this->begin(); it != this->end(); it++)
		{
			(*it)->OnUpdate(ts);
		}
		if (callback)
		{
			callback();
			callback = nullptr;
		}
	}

	void LayerStack::OnEvent(Hazel::Event& e)
	{
		for (auto it = this->rbegin(); it != this->rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void LayerStack::AttachLayer(Layer* layer)
	{
		layer->OnAttach();
		PushLayer(layer);
	}

	LayerStack::~LayerStack()
	{

	}
}