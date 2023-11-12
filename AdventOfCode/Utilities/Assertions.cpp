#include "pch.h"

#include "Assertions.h"

namespace Utilities
{
	void VerifyElseCrash(bool bAssert)
	{
		assert(bAssert);
		if (!bAssert)
		{
			std::terminate();
		}
	}

	void VerifyNotReached()
	{
		VerifyElseCrash(false);
	}
} // namespace Utilities