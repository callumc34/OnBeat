#pragma once
#include <OnBeat/Util/Template/Layer.h>
#include <OnBeat/Util/Loader/Loader.h>
#include <OnBeat/Config/Skin.h>

#define BindLoadingCallback(fn) (OnBeat::LoaderCallback)std::bind(fn, this)

namespace OnBeat
{
	class LoadingLayer : public Layer, public Loader
	{
		public:
			LoadingLayer(LoaderFunction fn, LoaderCallback callback, bool visible);
			~LoadingLayer();

			void OnUpdate(Hazel::Timestep ts) override;

		private:
			LoaderCallback callback;
			bool visible;

			Skin::Quad LoadingScreen;
	};
}