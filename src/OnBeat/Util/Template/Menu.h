#pragma once
#include <OnBeat/Util/Template/GLTextureSurface.h>
#include <OnBeat/Util/Template/Layer.h>
#include <Ultralight/Ultralight.h>

typedef struct GLFWcursor GLFWcursor;

namespace OnBeat
{
	class Menu : public Layer, public ultralight::LoadListener, public ultralight::ViewListener
	{
		public:
			Menu(const std::string& document, App* MainApp, std::string layerName);
			~Menu();

			void LoadDocument(const ultralight::String& document);

			void SetCursor(ultralight::Cursor cursor);

			void OnUpdate(Hazel::Timestep ms) override;
			void OnEvent(Hazel::Event& e) override;
			void OnDOMReady(ultralight::View* caller,
				uint64_t frame_id,
				bool is_main_frame,
				const ultralight::String& url) override;
			void OnChangeCursor(ultralight::View* caller, ultralight::Cursor cursor) override;
			bool OnMouseMove(Hazel::MouseMovedEvent& e);
			bool OnKeyPress(Hazel::KeyPressedEvent& e);
			bool OnKeyRelease(Hazel::KeyReleasedEvent& e);
			bool OnKeyTyped(Hazel::KeyTypedEvent& e);
			bool ResizeView(Hazel::WindowResizeEvent& e);

			virtual void UpdateMenu(Hazel::Timestep ms) = 0;

		protected:
			std::string DocumentPath;

		private:
			//Ultralight setup
			ultralight::Config Config;
			ultralight::RefPtr<ultralight::Renderer> renderer;
			ultralight::RefPtr<ultralight::View> view;
			std::unique_ptr<GLTextureSurfaceFactory> factory;

			struct GLFWCursors
			{
				// Create standard cursors
				GLFWcursor* cursor_ibeam;
				GLFWcursor* cursor_crosshair;
				GLFWcursor* cursor_hand;
				GLFWcursor* cursor_hresize;
				GLFWcursor* cursor_vresize;
			} Cursors;
	};
}