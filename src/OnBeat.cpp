#include <OnBeat/OnBeat.h>
#include <OnBeat/MusicLayer/MusicLayer.h>
#include <Hazel/Core/EntryPoint.h>

#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

OnBeat::OnBeat()
{
	//Temp test music
	OnSetGen gen(0, 1, 8, 5);
	auto all = gen.processFile("testing/testMusic/test.wav");
	auto beats = gen.findBeats(all);
	gen.createBeatFile(beats, "testing/graphing/testBeats.log");

	cwd = std::filesystem::current_path().string();
	Settings = Config::Settings((cwd + "/assets/user/UserConfig.json").c_str());

	SetWindowIcon();
	PushLayer(new MusicLayer(this, beats,
		900.0f, 44100, 512));
}

float OnBeat::pxToGlF(float px)
{
	return px / 100;
}

void OnBeat::SetWindowIcon()
{
	auto& window = Hazel::Application::Get().GetWindow();
	GLFWwindow* nativeWindow = static_cast<GLFWwindow*>(window.GetNativeWindow());

	std::string path = iconPath;

	int width, height, channels;
	//stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
	HZ_ASSERT(channels == 4, "Icon must be RGBA!");

	GLFWimage images[1];
	images[0].width = width;
	images[0].height = height;
	images[0].pixels = data;
	glfwSetWindowIcon(nativeWindow, 1, images);
	stbi_image_free(data);
}

OnBeat::~OnBeat()
{

}

Hazel::Application* Hazel::CreateApplication()
{
	return new OnBeat();
}