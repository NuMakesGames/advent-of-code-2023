#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle09B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle09.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle09B not yet solved!";
	}
} // namespace Puzzle09B