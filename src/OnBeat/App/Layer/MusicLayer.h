#pragma once
#include <OnBeat/Util/Template/Layer.h>
#include <OnBeat/Util/OnSetDetection/OnSetDetection.h>
#include <Hazel/Renderer/Shader.h>
#include <Hazel/Renderer/Renderer2D.h>

namespace OnBeat
{
	class MusicLayer : public Layer
	{
		public:
			//These need to all be variables from a style file when implemented

			MusicLayer(const char* file,
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

			std::unordered_map<int, std::vector<float>> beatHeights;

			Hazel::Ref<Hazel::Texture2D> beatTexture;
			Hazel::Ref<Hazel::Shader> flatColorShader;
			Hazel::Ref<Hazel::Shader> textureShader;
			Hazel::ShaderLibrary shaderLibrary;

			//Blit calculations
			AudioVector beats;
			float cameraVelocity;
			float cameraVelocityRatio;

			int sampleSize;
			double sampleRate;

			std::string musicFile;
	};
}