#include <OnBeat/Util/Discord/Integration.h>
#include <OnBeat/Util/Secrets/secrets.h>
#include <Hazel/Core/Log.h>
#include <iostream>

namespace OnBeat
{
	namespace Discord
	{
		Integration* Integration::instance = nullptr;

		Integration::Integration()
		{
			auto result = discord::Core::Create(CLIENT_ID, DiscordCreateFlags_Default, &core);
			State.core.reset(core);

			status = (bool)result;
			if (!State.core)
			{
				HZ_ERROR("Discord: Failed to instantiate discord core!");
				HZ_ERROR("Discord Error:", static_cast<int>(result));
			}

			State.core->SetLogHook(
				discord::LogLevel::Debug, [](discord::LogLevel level, const char* message)
				{
					HZ_INFO("Discord (", static_cast<uint32_t>(level), "): Message: ", message);
				}
			);

			State.core->UserManager().OnCurrentUserUpdate.Connect([&State=State]()
				{
					auto result = State.core->UserManager().GetCurrentUser(&State.CurrentUser);
					if (static_cast<int>(result) != 0)
					{
						HZ_WARN("Discord: Failed to get current user");
						HZ_WARN("Discord Error: ", static_cast<int>(result));
					}

					HZ_INFO("Discord: Current user updated: " + std::string(State.CurrentUser.GetUsername()) + "#"
						+ std::string(State.CurrentUser.GetDiscriminator()));
				}
			);

			//TODO(Callum): Register steam
			//activityManager.RegisterSteam(1938123);

		}

		Integration::~Integration()
		{

		}
	}
}