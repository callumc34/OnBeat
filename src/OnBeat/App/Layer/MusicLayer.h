#pragma once
#include <OnBeat/Util/Template/Layer.h>
#include <OnBeat/Util/OnSetDetection/OnSetDetection.h>
#include <OnBeat/Config/Skin.h>
#include <Hazel/Renderer/Shader.h>
#include <Hazel/Renderer/Renderer2D.h>

namespace OnBeat
{
	class MusicLayer : public Layer
	{
		public:
			//These need to all be variables from a style file when implemented

			MusicLayer(const std::string& file,
				float cameraVelocity, double sampleRate, int sampleSize);

			//OnEvents
			virtual void OnAttach() override;
			virtual void OnDetach() override;

			void EventUpdate(Hazel::Event& e) override;
			void OnUpdate(Hazel::Timestep ts) override;

			virtual void OnImGuiRender() override;

		private:

			OnSetDetection beatGen;

			//Textures & Shading
			void CreateBeatArea();
			void FindBeatHeights();
			void CreateBeats();

			Skin::MusicSkin skin;

			std::unordered_map<int, std::vector<float>> BeatHeights;

			//Blit calculations
			AudioVector beats;
			float CameraVelocity;

			int SampleSize;
			double SampleRate;

			std::string file;
	};
}