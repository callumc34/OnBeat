#include <OnBeat/Util/Loader/LoadingLayer/LoadingLayer.h>
#include <OnBeat/App/App.h>
#include <Hazel/Renderer/RenderCommand.h>
#include <Hazel/Renderer/Renderer2D.h>

namespace OnBeat
{
	LoadingLayer::LoadingLayer(LoaderFunction fn, LoaderCallback callback, bool visible) : Layer("Loading Layer"), Loader(fn),
		callback(callback), visible(visible)
	{
		//Attach self
		App::Get().GetLayerStack().AttachLayer(this);
		skin = App::Get().GetSettings().CurrentSkin.LoadingSkin;
		//Setup default bounds
		OnResize(
			Hazel::WindowResizeEvent(App::Get().GetWindow().GetWidth(), App::Get().GetWindow().GetHeight())
		);
	}

	bool LoadingLayer::OnResize(Hazel::WindowResizeEvent& e)
	{
		//Redefine loading bar bounds
		AnimationConfig.lower = (e.GetWidth() / (double)-210) + skin.LoadingAnimation.getScaleX();
		AnimationConfig.upper = (e.GetWidth() / (double)210) - skin.LoadingAnimation.getScaleX();
		return false;
	}

	void LoadingLayer::OnUpdate(Hazel::Timestep ts)
	{
		if (visible)
		{
			Hazel::Renderer2D::ResetStats();
			{
				Hazel::RenderCommand::SetClearColor(skin.ClearColour);
				Hazel::RenderCommand::Clear();
			}

			{
				Hazel::Renderer2D::BeginScene(*CameraController);
				//TODO(Callum): Render animation gif
				Render(ts.GetSeconds());
				Hazel::Renderer2D::EndScene();
			}
		}

		if (GetFinished())
		{
			//Pop self from layerstack
			Pop();
		}

	}

	void LoadingLayer::OnEvent(Hazel::Event& e)
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(LoadingLayer::OnResize));
		Layer::OnEvent(e);
	}

	void LoadingLayer::Render(float s)
	{
		//Background messes up when resized view
		skin.BackgroundTexture.draw(-0.5f);


		//Calculate bar location
		//Flip the velocity direction when out of bounds
		if (AnimationConfig.oldX < AnimationConfig.lower)
		{
			AnimationConfig.velocity *= -1;
			AnimationConfig.oldX = (float)AnimationConfig.lower;
		}
		if (AnimationConfig.oldX > AnimationConfig.upper)
		{
			AnimationConfig.velocity *= -1;
			AnimationConfig.oldX = (float)AnimationConfig.upper;
		}
		skin.LoadingAnimation.draw(1.0f, (AnimationConfig.oldX + (float)(AnimationConfig.velocity * s)));
		AnimationConfig.oldX += (float)(AnimationConfig.velocity * s);
	}


	void LoadingLayer::Pop()
	{
		//Pop self from layer stack when OnUpdate is completed
		LoaderCallback fn = [this]()
		{
			App::Get().GetLayerStack().PopLayer(this);
			callback();
		};
		App::Get().GetLayerStack().SetCallback(fn);
	}

	LoadingLayer::~LoadingLayer()
	{

	}
}