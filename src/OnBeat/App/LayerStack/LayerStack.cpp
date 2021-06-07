#include <OnBeat/App/LayerStack/LayerStack.h>

namespace OnBeat {

	LayerStack::LayerStack() : Layer("Layer Stack")
	{

	}

	void LayerStack::OnUpdate(Hazel::Timestep ms)
	{
		for (auto it = this->begin(); it != this->end(); it++)
		{
			static_cast<Hazel::Layer*>(*it)->OnUpdate(ms);
		}
	}

	LayerStack::~LayerStack()
	{

	}
}