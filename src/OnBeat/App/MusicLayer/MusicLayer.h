#pragma once
#include <OnBeat/Util/Loader/LoadingLayer/LoadingLayer.h>
#include <OnBeat/Util/OnSetDetection/OnSetDetection.h>
#include <Hazel/Renderer/Shader.h>
#include <Hazel/Renderer/Renderer2D.h>
#include <Hazel/Events/KeyEvent.h>

namespace OnBeat
{
	class MusicLayer : public Layer
	{
		public:
			//These need to all be variables from a style file when implemented

			MusicLayer(const std::string& file,
				float cameraVelocity, double sampleRate, int sampleSize);
			~MusicLayer();

			//OnEvents
			void OnAttach() override;
			void OnDetach() override;

			void OnEvent(Hazel::Event& e) override;
			void OnUpdate(Hazel::Timestep ts) override;

			virtual void OnImGuiRender() override;

			bool OnKeyRelease(Hazel::KeyReleasedEvent& e);

		private:
			//Textures & Shading
			void CreateBeatArea();
			void FindBeatHeights();
			void CreateBeats();

			OnSetDetection BeatGenerator;

			Skin::MusicSkin skin;

			std::unordered_map<int, std::vector<float>> BeatHeights;

			//Blit calculations
			AudioVector beats;
			float CameraVelocity;

			int SampleSize;
			double SampleRate;

			std::string file;

			void DiscordPresence();

			//Loading functionality
			void LoadLayer();
			void ClearLoadingLayer();
			LoadingLayer* LoadingLayer = nullptr;
			bool loaded = false;
	};
}