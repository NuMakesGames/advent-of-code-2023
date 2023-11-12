#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle22A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle22.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle22A not yet solved!";
	}
} // namespace Puzzle22A