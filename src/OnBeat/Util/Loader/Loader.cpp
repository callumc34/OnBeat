#include <OnBeat/Util/Loader/Loader.h>

namespace OnBeat
{
	Loader::Loader(LoaderFunction fn) : fn(fn)
	{
		finished = !fn;
		if (!finished)
		{
			ThreadClass.parent = this;
			thread = std::thread(ThreadClass);
		}
	}

	Loader::~Loader()
	{

	}
}