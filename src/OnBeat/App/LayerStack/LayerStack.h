#pragma once
#include <OnBeat/Util/Template/Layer.h>
#include <Hazel/Core/LayerStack.h>

namespace OnBeat
{
	class LayerStack : public Layer, public Hazel::LayerStack
	{
		public:
			LayerStack();
			~LayerStack();

			void OnUpdate(Hazel::Timestep ts) override;
			void OnEvent(Hazel::Event& e) override;


		private:

	};
}