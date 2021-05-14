#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <Hazel/Core/Application.h>
#include <Hazel/Core/KeyCodes.h>
#include <ultralight/KeyEvent.h>
#include <filesystem>

namespace OnBeat
{
	namespace Util
	{
		//Convert first 4 values in a json object to a vec4
		glm::vec4 arrayToVec4(nlohmann::json object)
		{
			return glm::vec4(object[0], object[1], object[2], object[3]);
		}

		//Convert string to Hazel float value
		float stringSizeToFloat(std::string val, Flag flag)
		{
			std::string type = val.substr(val.size() - 1, val.size());
			float n = std::atof(val.c_str());
			if (type == "%")
			{
				float scale = 1600.0f;
				if (flag == Flag::HEIGHT)
				{
					scale = Hazel::Application::Get().GetWindow().GetHeight();
				}
				if (flag == Flag::WIDTH)
				{
					scale = Hazel::Application::Get().GetWindow().GetWidth();
				}
				return n * scale / 10000;
			}
			else if (type == "px")
			{
				float scale;
				if (flag == Flag::HEIGHT)
				{
					scale = Hazel::Application::Get().GetWindow().GetHeight();
				}
				if (flag == Flag::WIDTH)
				{
					scale = Hazel::Application::Get().GetWindow().GetWidth();
				}
				return n / 100;
			}
			else
			{
				return n;
			}
		}

		//Check for file and create paths if makePath
		bool checkPath(std::string file, bool makePath)
		{
			if (!std::filesystem::exists(file))
			{
				if (makePath)
				{
					std::filesystem::create_directories(
						std::filesystem::path(file).parent_path());
				}

				return false;
			}

			return true;
		}

		using namespace ultralight;
		using namespace ultralight::KeyCodes;

		int HazelKeyCodeToUl(int key)
		{
			switch (key)
			{
				case Hazel::Key::Space: return GK_SPACE;
				case Hazel::Key::Apostrophe: return GK_OEM_7;
				case Hazel::Key::Comma: return GK_OEM_COMMA;
				case Hazel::Key::Minus: return GK_OEM_MINUS;
				case Hazel::Key::Period: return GK_OEM_PERIOD;
				case Hazel::Key::Slash: return GK_OEM_2;
				case Hazel::Key::D0: return GK_0;
				case Hazel::Key::D1: return GK_1;
				case Hazel::Key::D2: return GK_2;
				case Hazel::Key::D3: return GK_3;
				case Hazel::Key::D4: return GK_4;
				case Hazel::Key::D5: return GK_5;
				case Hazel::Key::D6: return GK_6;
				case Hazel::Key::D7: return GK_7;
				case Hazel::Key::D8: return GK_8;
				case Hazel::Key::D9: return GK_9;
				case Hazel::Key::Semicolon: return GK_OEM_1;
				case Hazel::Key::Equal: return GK_OEM_PLUS;
				case Hazel::Key::A: return GK_A;
				case Hazel::Key::B: return GK_B;
				case Hazel::Key::C: return GK_C;
				case Hazel::Key::D: return GK_D;
				case Hazel::Key::E: return GK_E;
				case Hazel::Key::F: return GK_F;
				case Hazel::Key::G: return GK_G;
				case Hazel::Key::H: return GK_H;
				case Hazel::Key::I: return GK_I;
				case Hazel::Key::J: return GK_J;
				case Hazel::Key::K: return GK_K;
				case Hazel::Key::L: return GK_L;
				case Hazel::Key::M: return GK_M;
				case Hazel::Key::N: return GK_N;
				case Hazel::Key::O: return GK_O;
				case Hazel::Key::P: return GK_P;
				case Hazel::Key::Q: return GK_Q;
				case Hazel::Key::R: return GK_R;
				case Hazel::Key::S: return GK_S;
				case Hazel::Key::T: return GK_T;
				case Hazel::Key::U: return GK_U;
				case Hazel::Key::V: return GK_V;
				case Hazel::Key::W: return GK_W;
				case Hazel::Key::X: return GK_X;
				case Hazel::Key::Y: return GK_Y;
				case Hazel::Key::Z: return GK_Z;
				case Hazel::Key::LeftBracket: return GK_OEM_4;
				case Hazel::Key::Backslash: return GK_OEM_5;
				case Hazel::Key::RightBracket: return GK_OEM_6;
				case Hazel::Key::GraveAccent: return GK_OEM_3;
				case Hazel::Key::World1: return GK_UNKNOWN;
				case Hazel::Key::World2: return GK_UNKNOWN;
				case Hazel::Key::Escape: return GK_ESCAPE;
				case Hazel::Key::Enter: return GK_RETURN;
				case Hazel::Key::Tab: return GK_TAB;
				case Hazel::Key::Backspace: return GK_BACK;
				case Hazel::Key::Insert: return GK_INSERT;
				case Hazel::Key::Delete: return GK_DELETE;
				case Hazel::Key::Right: return GK_RIGHT;
				case Hazel::Key::Left: return GK_LEFT;
				case Hazel::Key::Down: return GK_DOWN;
				case Hazel::Key::Up: return GK_UP;
				case Hazel::Key::PageUp: return GK_PRIOR;
				case Hazel::Key::PageDown: return GK_NEXT;
				case Hazel::Key::Home: return GK_HOME;
				case Hazel::Key::End: return GK_END;
				case Hazel::Key::CapsLock: return GK_CAPITAL;
				case Hazel::Key::ScrollLock: return GK_SCROLL;
				case Hazel::Key::NumLock: return GK_NUMLOCK;
				case Hazel::Key::PrintScreen: return GK_SNAPSHOT;
				case Hazel::Key::Pause: return GK_PAUSE;
				case Hazel::Key::F1: return GK_F1;
				case Hazel::Key::F2: return GK_F2;
				case Hazel::Key::F3: return GK_F3;
				case Hazel::Key::F4: return GK_F4;
				case Hazel::Key::F5: return GK_F5;
				case Hazel::Key::F6: return GK_F6;
				case Hazel::Key::F7: return GK_F7;
				case Hazel::Key::F8: return GK_F8;
				case Hazel::Key::F9: return GK_F9;
				case Hazel::Key::F10: return GK_F10;
				case Hazel::Key::F11: return GK_F11;
				case Hazel::Key::F12: return GK_F12;
				case Hazel::Key::F13: return GK_F13;
				case Hazel::Key::F14: return GK_F14;
				case Hazel::Key::F15: return GK_F15;
				case Hazel::Key::F16: return GK_F16;
				case Hazel::Key::F17: return GK_F17;
				case Hazel::Key::F18: return GK_F18;
				case Hazel::Key::F19: return GK_F19;
				case Hazel::Key::F20: return GK_F20;
				case Hazel::Key::F21: return GK_F21;
				case Hazel::Key::F22: return GK_F22;
				case Hazel::Key::F23: return GK_F23;
				case Hazel::Key::F24: return GK_F24;
				case Hazel::Key::F25: return GK_UNKNOWN;
				case Hazel::Key::KP0: return GK_NUMPAD0;
				case Hazel::Key::KP1: return GK_NUMPAD1;
				case Hazel::Key::KP2: return GK_NUMPAD2;
				case Hazel::Key::KP3: return GK_NUMPAD3;
				case Hazel::Key::KP4: return GK_NUMPAD4;
				case Hazel::Key::KP5: return GK_NUMPAD5;
				case Hazel::Key::KP6: return GK_NUMPAD6;
				case Hazel::Key::KP7: return GK_NUMPAD7;
				case Hazel::Key::KP8: return GK_NUMPAD8;
				case Hazel::Key::KP9: return GK_NUMPAD9;
				case Hazel::Key::KPDecimal: return GK_DECIMAL;
				case Hazel::Key::KPDivide: return GK_DIVIDE;
				case Hazel::Key::KPMultiply: return GK_MULTIPLY;
				case Hazel::Key::KPSubtract: return GK_SUBTRACT;
				case Hazel::Key::KPAdd: return GK_ADD;
				case Hazel::Key::KPEnter: return GK_RETURN;
				case Hazel::Key::KPEqual: return GK_OEM_PLUS;
				case Hazel::Key::LeftShift: return GK_SHIFT;
				case Hazel::Key::LeftControl: return GK_CONTROL;
				case Hazel::Key::LeftAlt: return GK_MENU;
				case Hazel::Key::LeftSuper: return GK_LWIN;
				case Hazel::Key::RightShift: return GK_SHIFT;
				case Hazel::Key::RightControl: return GK_CONTROL;
				case Hazel::Key::RightAlt: return GK_MENU;
				case Hazel::Key::RightSuper: return GK_RWIN;
				case Hazel::Key::Menu: return GK_UNKNOWN;
				default: return GK_UNKNOWN;
			}
		
		}

		MouseEvent::Button HazelMouseCodeToUl(int key)
		{
			switch (key)
			{
				case 0: return MouseEvent::kButton_Left;
				case 1: return MouseEvent::kButton_Middle;
				case 2: return MouseEvent::kButton_Right;
				default:return MouseEvent::kButton_None;
			}
		}
	}
}