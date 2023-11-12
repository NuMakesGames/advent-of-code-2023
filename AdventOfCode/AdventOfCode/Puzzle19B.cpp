#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle19B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle19.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle19B not yet solved!";
	}
} // namespace Puzzle19B