#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle09A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle09.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle09A not yet solved!";
	}
} // namespace Puzzle09A