#include <OnBeat/Util/Template/Menu.h>
#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <AppCore/Platform.h>
#include <Hazel/Renderer/Renderer2D.h>
#include <Hazel/Renderer/RenderCommand.h>
#include <GLFW/glfw3.h>

namespace ul = ultralight;

namespace OnBeat
{
	Menu::Menu(const std::string& document, std::string layerName)
		: Layer(layerName)
	{
		Config.resource_path = "assets/resources/";
		Config.use_gpu_renderer = false;
		Config.device_scale = 1.0;

		ul::Platform::instance().set_config(Config);
		ul::Platform::instance().set_font_loader(ul::GetPlatformFontLoader());
		ul::Platform::instance().set_file_system(ul::GetPlatformFileSystem("."));
		ul::Platform::instance().set_logger(ul::GetDefaultLogger("ul.log"));

		factory.reset(new GLTextureSurfaceFactory());
		ul::Platform::instance().set_surface_factory(factory.get());

		renderer = ul::Renderer::Create();

		// Create standard cursors
		Cursors.cursor_ibeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		Cursors.cursor_crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
		Cursors.cursor_hand = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
		Cursors.cursor_hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		Cursors.cursor_vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);

		auto& window = Hazel::Application::Get().GetWindow();
		view = renderer->CreateView(window.GetWidth(), window.GetHeight(), false, nullptr);
		view->set_load_listener(this);
		view->set_view_listener(this);

		LoadDocument("file:///" + ul::String(document.c_str()));
	}

	void Menu::LoadDocument(const ul::String& document)
	{
		view->LoadURL(document);
		view->Focus();
	}

	void Menu::SetCursor(ul::Cursor cursor)
	{
		//Todo fix Imgui resetting the cursor
		switch (cursor)
		{
			case ultralight::kCursor_Cross:
				glfwSetCursor(App::Get().GetNativeWindow(), Cursors.cursor_crosshair);
				break;
			case ultralight::kCursor_Hand:
				glfwSetCursor(App::Get().GetNativeWindow(), Cursors.cursor_hand);
				break;
			case ultralight::kCursor_IBeam:
				glfwSetCursor(App::Get().GetNativeWindow(), Cursors.cursor_ibeam);
				break;
			case ultralight::kCursor_EastWestResize:
				glfwSetCursor(App::Get().GetNativeWindow(), Cursors.cursor_hresize);
				break;
			case ultralight::kCursor_NorthSouthResize:
				glfwSetCursor(App::Get().GetNativeWindow(), Cursors.cursor_vresize);
				break;
			default:
				glfwSetCursor(App::Get().GetNativeWindow(), nullptr);
		}
	}

	void Menu::OnUpdate(Hazel::Timestep ms)
	{
		{
			Hazel::RenderCommand::SetClearColor({ 1.0f, 1.0f, 1.0f, 1.0f });
			Hazel::RenderCommand::Clear();
		}

		{
			renderer->Update();
			renderer->Render();

			GLTextureSurface* textureSurface = static_cast<GLTextureSurface*>(view->surface());

			if (textureSurface)
			{

				uint32_t width = textureSurface->width();
				uint32_t height = textureSurface->height();
				GLuint textureId = textureSurface->GetTextureAndSyncIfNeeded();

				GLubyte* data = new GLubyte[textureSurface->size()];

				glGetTextureImage(textureId, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureSurface->size(), data);

				Hazel::Ref<Hazel::Texture2D> texture = Hazel::Texture2D::Create(width, height);
				texture->SetData(data, textureSurface->size());

				Hazel::Renderer2D::BeginScene(*CameraController);

				Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { width / 100.0f, height / -100.0f }, texture);
				Hazel::Renderer2D::EndScene();
				delete[] data;
			}
		}

		UpdateMenu(ms);
	}

	void Menu::OnEvent(Hazel::Event& e)
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::MouseMovedEvent>(HZ_BIND_EVENT_FN(Menu::OnMouseMove));
		dispatcher.Dispatch<Hazel::MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(Menu::OnMouseButtonPress));
		dispatcher.Dispatch<Hazel::MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(Menu::OnMouseButtonRelease));
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(Menu::OnKeyPress));
		dispatcher.Dispatch<Hazel::KeyReleasedEvent>(HZ_BIND_EVENT_FN(Menu::OnKeyRelease));
		dispatcher.Dispatch<Hazel::KeyTypedEvent>(HZ_BIND_EVENT_FN(Menu::OnKeyTyped));
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(Menu::ResizeView));
		Layer::OnEvent(e);
	}

	void Menu::OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor)
	{
		SetCursor(cursor);
	}

	void Menu::OnAddConsoleMessage(ultralight::View* caller, ultralight::MessageSource source,
		ultralight::MessageLevel level, const ultralight::String& message,
		uint32_t line_number, uint32_t column_number,
		const ultralight::String& source_id)
	{
#ifdef _DEBUG
		HZ_INFO(message.utf8().data());
		return;
#else
		return;
#endif
	}

	bool Menu::OnMouseMove(Hazel::MouseMovedEvent& e)
	{
		ul::MouseEvent evt;
		evt.type = ul::MouseEvent::kType_MouseMoved;
		evt.x = e.GetX();
		evt.y = e.GetY();
		CurrentMouse = { e.GetX(), e.GetY() };

		view->FireMouseEvent(evt);
		return true;
	}

	bool Menu::OnMouseButtonPress(Hazel::MouseButtonPressedEvent& e)
	{
		ul::MouseEvent evt;
		evt.type = ul::MouseEvent::kType_MouseDown;
		evt.button = Util::HazelMouseCodeToUl(e.GetMouseButton());
		evt.x = CurrentMouse.x;
		evt.y = CurrentMouse.y;

		view->FireMouseEvent(evt);
		return true;
	}

	bool Menu::OnMouseButtonRelease(Hazel::MouseButtonReleasedEvent& e)
	{
		ul::MouseEvent evt;
		evt.type = ul::MouseEvent::kType_MouseUp;
		evt.button = Util::HazelMouseCodeToUl(e.GetMouseButton());
		evt.x = CurrentMouse.x;
		evt.y = CurrentMouse.y;
	
		view->FireMouseEvent(evt);
		return true;
	}

	bool Menu::OnKeyPress(Hazel::KeyPressedEvent& e)
	{
		ul::KeyEvent evt;
		evt.type = ul::KeyEvent::kType_RawKeyDown;
		evt.virtual_key_code = Util::HazelKeyCodeToUl(e.GetKeyCode());
		evt.native_key_code = 0;
		evt.modifiers = 0;

		ul::GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);
		view->FireKeyEvent(evt);

		return true;
	}

	bool Menu::OnKeyRelease(Hazel::KeyReleasedEvent& e)
	{
		ul::KeyEvent evt;
		evt.type = ul::KeyEvent::kType_KeyUp;
		evt.virtual_key_code = Util::HazelKeyCodeToUl(e.GetKeyCode());
		evt.native_key_code = 0;
		evt.modifiers = 0;

		ul::GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);
		view->FireKeyEvent(evt);
		return true;
	}

	bool Menu::OnKeyTyped(Hazel::KeyTypedEvent& e)
	{
		ul::KeyEvent evt;
		evt.type = ul::KeyEvent::kType_Char;
		evt.text = std::string(1, (char)e.GetKeyCode()).c_str();
		evt.unmodified_text = evt.text;

		view->FireKeyEvent(evt);
		return true;
	}

	bool Menu::ResizeView(Hazel::WindowResizeEvent& e)
	{
		view->Resize(e.GetWidth(), e.GetHeight());
		return true;
	}

	Menu::~Menu()
	{
		glfwDestroyCursor(Cursors.cursor_ibeam);
		glfwDestroyCursor(Cursors.cursor_crosshair);
		glfwDestroyCursor(Cursors.cursor_hand);
		glfwDestroyCursor(Cursors.cursor_hresize);
		glfwDestroyCursor(Cursors.cursor_vresize);
	}
}