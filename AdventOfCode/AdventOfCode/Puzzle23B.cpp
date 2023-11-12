#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle23B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle23.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle23B not yet solved!";
	}
} // namespace Puzzle23B