#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle04B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle04.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle04B not yet solved!";
	}
} // namespace Puzzle04B