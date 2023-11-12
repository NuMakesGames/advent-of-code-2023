#pragma once

namespace Utilities
{
	// Crashes the program if the assertion is false.
	void VerifyElseCrash(bool bAssert);

	// Crashes the program if executed. Use to annotate code locations expected to be unreachable.
	void VerifyNotReached();
}