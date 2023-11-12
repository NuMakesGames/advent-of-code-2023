#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle02A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle02.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle02A not yet solved!";
	}
} // namespace Puzzle02A