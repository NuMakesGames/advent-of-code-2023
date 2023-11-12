#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle14A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle14.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle14A not yet solved!";
	}
} // namespace Puzzle14A