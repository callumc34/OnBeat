#pragma once
#include <OnBeat/Util/Template/Layer.h>
#include <OnBeat/Util/Loader/Loader.h>
#include <OnBeat/Config/Skin.h>

#define BindLoadingFunction(fn) (OnBeat::LoaderFunction)std::bind(fn, this)
#define BindLoadingCallback(fn) (OnBeat::LoaderCallback)std::bind(fn, this)

namespace OnBeat
{
	class LoadingLayer : public Layer, public Loader
	{
		public:
			LoadingLayer(LoaderFunction fn, LoaderCallback callback, bool visible);
			~LoadingLayer();

			bool OnResize(Hazel::WindowResizeEvent& e);
			void OnUpdate(Hazel::Timestep ts) override;
			void OnEvent(Hazel::Event& e) override;

			void Pop();

		private:
			void Render(float s);

			LoaderCallback callback;
			bool visible;

			Skin::LoadingSkin skin;
			struct
			{
				double velocity = 4;
				float oldX = 0.0f;
				double lower;
				double upper;
			} AnimationConfig;
	};
}