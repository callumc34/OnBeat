#pragma once
#include <Hazel.h>

#include <OnBeat/OnBeat.h>
#include <OnBeat/OnSetGen/OnSetGen.h>
#include <OnBeat/Config/OnBeatConfig.h>

class MusicLayer : public Hazel::Layer
{
	public:
		//These need to all be variables from a style file when implemented

		MusicLayer(OnBeat* app, const char* file,
			float cameraVelocity, double sampleRate, int sampleSize);

		//OnEvents
		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnEvent(Hazel::Event& e) override;
		void OnUpdate(Hazel::Timestep ts) override;
		bool OnWindowResize(Hazel::WindowResizeEvent& e);

		virtual void OnImGuiRender() override;

	private:
		void CreateCamera(uint32_t width, uint32_t height);

		Hazel::Scope<Hazel::OrthographicCamera> cameraController;

		OnBeat* app;
		OnSetGen beatGen;

		//Textures & Shading
		void CreateBeatArea();
		void FindBeatHeights();
		void CreateBeats();

		Config::Skin::MusicSkin skin;

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