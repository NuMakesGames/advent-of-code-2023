#pragma once

namespace Utilities
{
	// Crashes the program if the assertion is false.
	void VerifyElseCrashImpl(bool bAssert, const char* condition, std::source_location loc = std::source_location::current());

	// Crashes the program if executed. Use to annotate code locations expected to be unreachable.
	void VerifyNotReached(std::source_location loc = std::source_location::current());
} // namespace Utilities

#define VerifyElseCrash(bAssert)                                                            \
	do                                                                                      \
	{                                                                                       \
		Utilities::VerifyElseCrashImpl(bAssert, #bAssert, std::source_location::current()); \
	}                                                                                       \
	while (false)