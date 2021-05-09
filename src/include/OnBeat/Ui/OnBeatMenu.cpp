#include <OnBeat/Ui/OnBeatMenu.h>
#include <AppCore/Platform.h>

namespace ul = ultralight;

OnBeatMenu::OnBeatMenu(const std::string& document) : Layer("OnBeatMenu"),
	renderer(ul::Renderer::Create()), documentPath(document)
{
	//ul config setup
	config.resource_path = "assets/resources/";
	config.use_gpu_renderer = false;
	config.device_scale = 1.0;

	//ul platform setup
	ul::Platform::instance().set_config(config);
	ul::Platform::instance().set_font_loader(ul::GetPlatformFontLoader());
	ul::Platform::instance().set_file_system(ul::GetPlatformFileSystem("."));
	ul::Platform::instance().set_logger(ul::GetDefaultLogger("ul.log"));

	factory.reset(new GLTextureSurfaceFactory());
	ul::Platform::instance().set_surface_factory(factory.get());

	auto& window = Hazel::Application::Get().GetWindow();
	view = renderer->CreateView(window.GetWidth(), window.GetHeight(), false, nullptr);
	CreateCamera(window.GetWidth(), window.GetHeight());

	LoadDocument("file:///" + ul::String(documentPath.c_str()));
}

void OnBeatMenu::LoadDocument(const ul::String& document)
{
	view->LoadURL(document);
	view->Focus();
}

void OnBeatMenu::OnUpdate(Hazel::Timestep ms)
{
	{
		Hazel::RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		Hazel::RenderCommand::Clear();
	}

	{
		renderer->Update();
		renderer->Render();

		ul::Surface* surface = view->surface();

		if (surface)
		{
			GLPBOTextureSurface* textureSurface = static_cast<GLPBOTextureSurface*>(surface);
			
			uint32_t width = textureSurface->width();
			uint32_t height = textureSurface->height();
			GLuint textureId = textureSurface->GetTextureAndSyncIfNeeded();

			//GLuint fboId = 0;
			//glGenFramebuffers(1, &fboId);
			//glBindFramebuffer(GL_READ_FRAMEBUFFER, fboId);
			//glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			//	GL_TEXTURE_2D, textureId, 0);

			//glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);  // if not already bound
			//glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
			//	GL_COLOR_BUFFER_BIT, GL_NEAREST);

			GLubyte* data = new GLubyte[width * height * 4];

			glGetTextureImage(textureId, 0, GL_RGBA, GL_UNSIGNED_BYTE, width * height * 4, data);

			Hazel::Ref<Hazel::Texture2D> texture = Hazel::Texture2D::Create(width, height);
			texture->SetData(data, width * height * 4);

			Hazel::Renderer2D::BeginScene(*cameraController);

			Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { width / 100.0f, height / -100.0f }, texture);
			Hazel::Renderer2D::EndScene();
			delete[] data;
		}
	}
}

void OnBeatMenu::OnEvent(Hazel::Event& e)
{
	Hazel::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(OnBeatMenu::OnWindowResize));
}

bool OnBeatMenu::OnWindowResize(Hazel::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return true;
}

void OnBeatMenu::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;

	float camHeight = (float)height / 200;
	float bottom = -camHeight;
	float top = camHeight;
	float left = bottom * aspectRatio;
	float right = top * aspectRatio;
	cameraController = Hazel::CreateScope<Hazel::OrthographicCamera>(left, right, bottom, top);
	view->Resize(width, height);
}