#include <OnBeat/Util/Loader/Loader.h>

namespace OnBeat
{
	Loader::Loader(LoaderFunction fn) : fn(fn)
	{
		finished = !fn;
		if (!finished)
		{
			InternalThread.parent = this;
			thread = std::thread(InternalThread);
		}
	}

	Loader::~Loader()
	{
		thread.join();
	}
}