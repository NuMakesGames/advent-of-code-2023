#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle10B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle10.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle10B not yet solved!";
	}
} // namespace Puzzle10B