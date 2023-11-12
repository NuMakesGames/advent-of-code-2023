#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle25A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle25.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle25A not yet solved!";
	}
} // namespace Puzzle25A