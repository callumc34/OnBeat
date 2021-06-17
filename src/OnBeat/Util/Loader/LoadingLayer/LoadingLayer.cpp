#include <OnBeat/Util/Loader/LoadingLayer/LoadingLayer.h>
#include <OnBeat/App/App.h>
#include <Hazel/Renderer/RenderCommand.h>
#include <Hazel/Renderer/Renderer2D.h>

namespace OnBeat
{
	LoadingLayer::LoadingLayer(LoaderFunction fn, LoaderCallback callback, bool visible) : Layer("Loading Layer"), Loader(fn),
		callback(callback), visible(visible)
	{
		App::Get().GetLayerStack().AttachLayer(this);
		LoadingScreen = App::Get().GetSettings().CurrentSkin.LoadingScreen;
	}

	void LoadingLayer::OnUpdate(Hazel::Timestep ts)
	{
		if (visible)
		{
			Hazel::Renderer2D::ResetStats();
			{
				Hazel::RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
				Hazel::RenderCommand::Clear();
			}

			{
				Hazel::Renderer2D::BeginScene(*CameraController);
				//TODO(Callum): Render animation gif
				LoadingScreen.draw();
				Hazel::Renderer2D::EndScene();
			}
		}

		if (GetFinished())
		{
			callback();
			Pop();
		}

	}

	void LoadingLayer::Pop()
	{
		LoaderCallback fn = [this]()
		{
			App::Get().GetLayerStack().PopLayer(this);
		};
		App::Get().GetLayerStack().SetCallback(fn);
	}

	LoadingLayer::~LoadingLayer()
	{

	}
}