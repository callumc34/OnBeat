#pragma once
#include <functional>
#include <thread>

namespace OnBeat
{

	typedef std::function<void()> LoaderFunction;
	typedef std::function<void()> LoaderCallback;

	class Loader
	{
		public:
			Loader(LoaderFunction fn);
			~Loader();

			bool GetFinished() { return finished; }

		protected:
			LoaderFunction fn;
		
		private:
			struct InternalFunction
			{
				void operator()()
				{
					if (parent->fn)
					{
						parent->fn();
						parent->finished = true;
					}
				};

				Loader* parent;
				
			} ThreadClass;

			std::thread thread;

			bool finished;

			friend InternalFunction;
	};
}