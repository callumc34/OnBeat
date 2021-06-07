#include <OnBeat/App/LayerStack/LayerStack.h>

namespace OnBeat {

	LayerStack::LayerStack() : Layer("Layer Stack")
	{

	}
	//TODO(Callum): Fix vector errors when Popping and Pushing
	void LayerStack::OnUpdate(Hazel::Timestep ts)
	{
		for (auto it = this->begin(); it != this->end(); it++)
		{
			(*it)->OnUpdate(ts);
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

	LayerStack::~LayerStack()
	{

	}
}