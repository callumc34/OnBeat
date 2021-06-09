#pragma once
#include <OnBeat/Util/Template/Layer.h>
#include <Hazel/Core/LayerStack.h>

#define BindPostUpdateCallback(fn) (std::function<void()>)std::bind(fn, this)

namespace OnBeat
{
	class LayerStack : public Layer, public Hazel::LayerStack
	{
		typedef std::function<void()> PostUpdateCallback;
		public:
			LayerStack();
			~LayerStack();

			void OnUpdate(Hazel::Timestep ts) override;
			void OnEvent(Hazel::Event& e) override;

			void AttachLayer(Layer* layer);

			void SetCallback(PostUpdateCallback fn) { callback = fn; }

		private:
			PostUpdateCallback callback;
	};
}