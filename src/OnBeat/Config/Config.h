#pragma once
#include <OnBeat/Config/Skin.h>
#include <Hazel/Core/KeyCodes.h>

#define OB_DEFAULT_SKIN "assets/user/skins/Default"
#define OB_DEFAULT_SETTINGS "assets/user/DefaultConfig.json"
#define OB_SETTINGS "assets/user/UserConfig.json"
#define OB_MAIN_MENU "/menu/MainMenu/main.html"

//Generic settings to be checked against upon swapping values
//Bit of an issue if one of the settings has to be int16_t max
#define OB_UNDEFINED_INT INT16_MAX
#define OB_UNDEFINED_KEY "Undefined"

//magic_enum throws error if this is not defined
int main(int argc, char** argv);

namespace OnBeat
{
	namespace Config
	{

		//Had to redefine Keys to work with magic_cast
		enum Keys : uint16_t
		{
			// From glfw3.h
			Space = 32,
			Apostrophe = 39, /* ' */
			Comma = 44, /* , */
			Minus = 45, /* - */
			Period = 46, /* . */
			Slash = 47, /* / */

			D0 = 48, /* 0 */
			D1 = 49, /* 1 */
			D2 = 50, /* 2 */
			D3 = 51, /* 3 */
			D4 = 52, /* 4 */
			D5 = 53, /* 5 */
			D6 = 54, /* 6 */
			D7 = 55, /* 7 */
			D8 = 56, /* 8 */
			D9 = 57, /* 9 */

			Semicolon = 59, /* ; */
			Equal = 61, /* = */

			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,

			LeftBracket = 91,  /* [ */
			Backslash = 92,  /* \ */
			RightBracket = 93,  /* ] */
			GraveAccent = 96,  /* ` */

			World1 = 161, /* non-US #1 */
			World2 = 162, /* non-US #2 */

			/* Function keys */
			Escape = 256,
			Enter = 257,
			Tab = 258,
			Backspace = 259,
			Insert = 260,
			Delete = 261,
			Right = 262,
			Left = 263,
			Down = 264,
			Up = 265,
			PageUp = 266,
			PageDown = 267,
			Home = 268,
			End = 269,
			CapsLock = 280,
			ScrollLock = 281,
			NumLock = 282,
			PrintScreen = 283,
			Pause = 284,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			F13 = 302,
			F14 = 303,
			F15 = 304,
			F16 = 305,
			F17 = 306,
			F18 = 307,
			F19 = 308,
			F20 = 309,
			F21 = 310,
			F22 = 311,
			F23 = 312,
			F24 = 313,
			F25 = 314,

			/* Keypad */
			KP0 = 320,
			KP1 = 321,
			KP2 = 322,
			KP3 = 323,
			KP4 = 324,
			KP5 = 325,
			KP6 = 326,
			KP7 = 327,
			KP8 = 328,
			KP9 = 329,
			KPDecimal = 330,
			KPDivide = 331,
			KPMultiply = 332,
			KPSubtract = 333,
			KPAdd = 334,
			KPEnter = 335,
			KPEqual = 336,

			LeftShift = 340,
			LeftControl = 341,
			LeftAlt = 342,
			LeftSuper = 343,
			RightShift = 344,
			RightControl = 345,
			RightAlt = 346,
			RightSuper = 347,
			Menu = 348,

			Undefined = 500,
		};

		//Settings structs
		struct VideoConfig
		{
			uint16_t 
				DisplayWidth = OB_UNDEFINED_INT,
				DisplayHeight = OB_UNDEFINED_INT,
				VSync = OB_UNDEFINED_INT,
				FpsCap = OB_UNDEFINED_INT;
			int16_t Fullscreen = OB_UNDEFINED_INT;
		};

		void setResolution(VideoConfig& c, const std::string& res);
		void to_json(nlohmann::json& j, const VideoConfig& c);
		void from_json(const nlohmann::json& j, VideoConfig& c);

		struct InputConfig
		{
			Keys
				COLUMN_1 = Keys::Undefined,
				COLUMN_2 = Keys::Undefined,
				COLUMN_3 = Keys::Undefined,
				COLUMN_4 = Keys::Undefined,
				PAUSE = Keys::Undefined
				;				
		};

		void to_json(nlohmann::json& j, const InputConfig& c);
		void from_json(const nlohmann::json& j, InputConfig& c);

		struct AudioConfig
		{
			float Volume = OB_UNDEFINED_INT;
		};

		void to_json(nlohmann::json& j, const AudioConfig& c);
		void from_json(const nlohmann::json& j, AudioConfig& c);

		struct GameConfig
		{
			uint16_t
				CameraVelocity = OB_UNDEFINED_INT,
				MeanWindow = OB_UNDEFINED_INT,
				MaximaWindow = OB_UNDEFINED_INT;
			double
				ThresholdConstant = OB_UNDEFINED_INT,
				ThresholdMultiple = OB_UNDEFINED_INT;
			Skin::AppSkin Skin;
		};

		void to_json(nlohmann::json& j, const GameConfig& c);
		void from_json(const nlohmann::json& j, GameConfig& c);

		struct Settings
		{
			static Settings Create(const std::string& path);
			
			VideoConfig Video;
			InputConfig Input;
			AudioConfig Audio;
			GameConfig Game;
		};

		void to_json(nlohmann::json& j, const Settings& c);

		void from_json(const nlohmann::json& j, Settings& c);

		bool swapSettings(Settings& newS, const Settings& oldS);

		bool validateSettings(const Settings& c, bool allowUndefined = true);

	}
}