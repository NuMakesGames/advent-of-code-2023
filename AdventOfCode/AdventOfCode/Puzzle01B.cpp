#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle01B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle01.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle01B not yet solved!";
	}
} // namespace Puzzle01B