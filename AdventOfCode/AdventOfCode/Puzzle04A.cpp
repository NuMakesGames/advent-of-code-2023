#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle04A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle04.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle04A not yet solved!";
	}
} // namespace Puzzle04A