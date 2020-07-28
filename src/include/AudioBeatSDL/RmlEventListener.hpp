#include <iostream>
#include <SDL2/SDL.h>
#include <RmlUi/Core.h>

struct UserEvent {
	enum Code {
		LAUNCH_GAME = 0x1001,
		CREATE_BLIT = 0x1002,
		FINISHED_RHYTHM = 0x1003,
	};
};

#pragma region RmlEvenListener
//Process custom events
class RmlEventListener : public Rml::Core::EventListener {
public:
	RmlEventListener() {};
	~RmlEventListener() {};

	void ProcessEvent(Rml::Core::Event& rmlEvent) {
		Rml::Core::String action = rmlEvent.GetTargetElement()->GetId();
		SDL_Event sdlEvent = newSDLEvent();

		if (sdlEvent.type == NULL) {
			std::cerr << "Error creating new SDL Event...\n";
			return;
		}

		if (action == "ExitGame") { //Until I figure out the correct way to do this
			sdlEvent.type = 0x100;
		}


		else if (action == "LaunchGame") {
			sdlEvent.user.code = UserEvent::Code::LAUNCH_GAME;
			
		}


		SDL_PushEvent(&sdlEvent);
	}

private:
	SDL_Event newSDLEvent() {

		SDL_Event sdlEvent;

		if (gameEventType != ((Uint32)-1)) {
			SDL_memset(&sdlEvent, 0, sizeof(sdlEvent));
			sdlEvent.type = gameEventType;
			sdlEvent.user.code = NULL;
			sdlEvent.user.data1 = NULL;
			sdlEvent.user.data2 = NULL;
		}
		else {
			std::cerr << "Error registering user event...\n";
			sdlEvent.user.type = NULL;
			
		}
		return sdlEvent;
	}

	Uint32 gameEventType = SDL_RegisterEvents(1);
};
#pragma endregion RmlEventListener