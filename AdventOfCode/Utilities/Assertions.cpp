#include "pch.h"

#include "Assertions.h"
#include "Console.h"

namespace Utilities
{
	void VerifyElseCrashImpl(bool bAssert, const char* condition, std::source_location loc)
	{
		assert(bAssert);
		if (!bAssert)
		{
			{
				ScopedConsoleTextColor textColor(ConsoleForegroundColor::IntenseRed);
				std::cout << "\nAssertion failed ";

				SetConsoleTextColor(ConsoleForegroundColor::White);
				std::cout << "at ";

				SetConsoleTextColor(ConsoleForegroundColor::IntenseYellow);
				std::cout << loc.file_name();

				SetConsoleTextColor(ConsoleForegroundColor::White);
				std::cout << "(";

				SetConsoleTextColor(ConsoleForegroundColor::IntenseMagenta);
				std::cout << loc.line();

				SetConsoleTextColor(ConsoleForegroundColor::White);
				std::cout << "): ";
					
				SetConsoleTextColor(ConsoleForegroundColor::IntenseRed);
				std::cout << condition << '\n';
			}

			std::terminate();
		}
	}

	void VerifyNotReached(std::source_location loc)
	{
		VerifyElseCrashImpl(false, "false", loc);
	}
} // namespace Utilities
