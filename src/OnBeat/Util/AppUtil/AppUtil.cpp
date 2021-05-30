#include <OnBeat/Util/AppUtil/AppUtil.h>
#include <Hazel/Core/Application.h>
#include <Hazel/Core/KeyCodes.h>
#include <ultralight/KeyEvent.h>
#include <filesystem>

namespace ultralight
{
	void to_json(nlohmann::json& j, const JSObject& obj)
	{
		JSPropertyNameArrayRef names = JSObjectCopyPropertyNames(obj.context(), obj);
		size_t len = JSPropertyNameArrayGetCount(names);
		for (int i = 0; i < len; i++)
		{
			JSString propertyName = JSPropertyNameArrayGetNameAtIndex(names, i);
			String ulString = propertyName;

			JSValueRef e = nullptr;
			JSValue val = JSObjectGetProperty(obj.context(), obj, propertyName, &e);
			String s = val.ToString();

			if (!e)
			{

				j[ulString.utf8().data()] = s.utf8().data();
			}
		}
	}

	void from_json(const nlohmann::json& j, JSObject& obj)
	{
		obj = (JSObjectRef)JSValueMakeFromJSONString(obj.context(), JSString(j.dump().c_str()));
	}
}

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

		int UlKeyCodeToHazel(int key)
		{
			switch (key)
			{
				case GK_SPACE: return Hazel::Key::Space;
				case GK_OEM_7: return Hazel::Key::Apostrophe;
				case GK_OEM_COMMA: return Hazel::Key::Comma;
				case GK_OEM_MINUS: return Hazel::Key::Minus;
				case GK_OEM_PERIOD: return Hazel::Key::Period;
				case GK_OEM_2: return Hazel::Key::Slash;
				case GK_0: return Hazel::Key::D0;
				case GK_1: return Hazel::Key::D1;
				case GK_2: return Hazel::Key::D2;
				case GK_3: return Hazel::Key::D3;
				case GK_4: return Hazel::Key::D4;
				case GK_5: return Hazel::Key::D5;
				case GK_6: return Hazel::Key::D6;
				case GK_7: return Hazel::Key::D7;
				case GK_8: return Hazel::Key::D8;
				case GK_9: return Hazel::Key::D9;
				case GK_OEM_1: return Hazel::Key::Semicolon;
				case GK_OEM_PLUS: return Hazel::Key::Equal;
				case GK_A: return Hazel::Key::A;
				case GK_B: return Hazel::Key::B;
				case GK_C: return Hazel::Key::C;
				case GK_D: return Hazel::Key::D;
				case GK_E: return Hazel::Key::E;
				case GK_F: return Hazel::Key::F;
				case GK_G: return Hazel::Key::G;
				case GK_H: return Hazel::Key::H;
				case GK_I: return Hazel::Key::I;
				case GK_J: return Hazel::Key::J;
				case GK_K: return Hazel::Key::K;
				case GK_L: return Hazel::Key::L;
				case GK_M: return Hazel::Key::M;
				case GK_N: return Hazel::Key::N;
				case GK_O: return Hazel::Key::O;
				case GK_P: return Hazel::Key::P;
				case GK_Q: return Hazel::Key::Q;
				case GK_R: return Hazel::Key::R;
				case GK_S: return Hazel::Key::S;
				case GK_T: return Hazel::Key::T;
				case GK_U: return Hazel::Key::U;
				case GK_V: return Hazel::Key::V;
				case GK_W: return Hazel::Key::W;
				case GK_X: return Hazel::Key::X;
				case GK_Y: return Hazel::Key::Y;
				case GK_Z: return Hazel::Key::Z;
				case GK_OEM_4: return Hazel::Key::LeftBracket;
				case GK_OEM_5: return Hazel::Key::Backslash;
				case GK_OEM_6: return Hazel::Key::RightBracket;
				case GK_OEM_3: return Hazel::Key::GraveAccent;
				case GK_ESCAPE: return Hazel::Key::Escape;
				case GK_RETURN: return Hazel::Key::Enter;
				case GK_TAB: return Hazel::Key::Tab;
				case GK_BACK: return Hazel::Key::Backspace;
				case GK_INSERT: return Hazel::Key::Insert;
				case GK_DELETE: return Hazel::Key::Delete;
				case GK_RIGHT: return Hazel::Key::Right;
				case GK_LEFT: return Hazel::Key::Left;
				case GK_DOWN: return Hazel::Key::Down;
				case GK_UP: return Hazel::Key::Up;
				case GK_PRIOR: return Hazel::Key::PageUp;
				case GK_NEXT: return Hazel::Key::PageDown;
				case GK_HOME: return Hazel::Key::Home;
				case GK_END: return Hazel::Key::End;
				case GK_CAPITAL: return Hazel::Key::CapsLock;
				case GK_SCROLL: return Hazel::Key::ScrollLock;
				case GK_NUMLOCK: return Hazel::Key::NumLock;
				case GK_SNAPSHOT: return Hazel::Key::PrintScreen;
				case GK_PAUSE: return Hazel::Key::Pause;
				case GK_F1: return Hazel::Key::F1;
				case GK_F2: return Hazel::Key::F2;
				case GK_F3: return Hazel::Key::F3;
				case GK_F4: return Hazel::Key::F4;
				case GK_F5: return Hazel::Key::F5;
				case GK_F6: return Hazel::Key::F6;
				case GK_F7: return Hazel::Key::F7;
				case GK_F8: return Hazel::Key::F8;
				case GK_F9: return Hazel::Key::F9;
				case GK_F10: return Hazel::Key::F10;
				case GK_F11: return Hazel::Key::F11;
				case GK_F12: return Hazel::Key::F12;
				case GK_F13: return Hazel::Key::F13;
				case GK_F14: return Hazel::Key::F14;
				case GK_F15: return Hazel::Key::F15;
				case GK_F16: return Hazel::Key::F16;
				case GK_F17: return Hazel::Key::F17;
				case GK_F18: return Hazel::Key::F18;
				case GK_F19: return Hazel::Key::F19;
				case GK_F20: return Hazel::Key::F20;
				case GK_F21: return Hazel::Key::F21;
				case GK_F22: return Hazel::Key::F22;
				case GK_F23: return Hazel::Key::F23;
				case GK_F24: return Hazel::Key::F24;
				case GK_UNKNOWN: return Hazel::Key::F25;
				case GK_NUMPAD0: return Hazel::Key::KP0;
				case GK_NUMPAD1: return Hazel::Key::KP1;
				case GK_NUMPAD2: return Hazel::Key::KP2;
				case GK_NUMPAD3: return Hazel::Key::KP3;
				case GK_NUMPAD4: return Hazel::Key::KP4;
				case GK_NUMPAD5: return Hazel::Key::KP5;
				case GK_NUMPAD6: return Hazel::Key::KP6;
				case GK_NUMPAD7: return Hazel::Key::KP7;
				case GK_NUMPAD8: return Hazel::Key::KP8;
				case GK_NUMPAD9: return Hazel::Key::KP9;
				case GK_DECIMAL: return Hazel::Key::KPDecimal;
				case GK_DIVIDE: return Hazel::Key::KPDivide;
				case GK_MULTIPLY: return Hazel::Key::KPMultiply;
				case GK_SUBTRACT: return Hazel::Key::KPSubtract;
				case GK_ADD: return Hazel::Key::KPAdd;
				case GK_SHIFT: return Hazel::Key::LeftShift;
				case GK_CONTROL: return Hazel::Key::LeftControl;
				case GK_MENU: return Hazel::Key::LeftAlt;
				case GK_LWIN: return Hazel::Key::LeftSuper;
				case GK_RWIN: return Hazel::Key::RightSuper;
				default: return -1;
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