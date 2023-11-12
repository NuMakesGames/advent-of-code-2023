#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle13A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle13.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle13A not yet solved!";
	}
} // namespace Puzzle13A