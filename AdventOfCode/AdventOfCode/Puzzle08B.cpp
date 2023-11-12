#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle08B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle08.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle08B not yet solved!";
	}
} // namespace Puzzle08B