#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle20B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle20.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle20B not yet solved!";
	}
} // namespace Puzzle20B