#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle05B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle05.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle05B not yet solved!";
	}
} // namespace Puzzle05B