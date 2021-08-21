#pragma once
#include <Discord/discord.h>

namespace OnBeat
{
	namespace Discord
	{
		class Integration
		{
			private:
				static Integration* instance;

				struct Data
				{
					discord::User CurrentUser;
					std::unique_ptr<discord::Core> core;
					discord::Activity activity;
				} State;

				Integration();
				~Integration();

				discord::Core* core{};

				bool status = false;

			public:
				static Integration& Get()
				{
					if (instance == nullptr)
						instance = new Integration();

					return *instance;
						
				}

				const bool GetStatus() const { return status; }
				const Data& GetState() const { return State; }
		};
	}
}
