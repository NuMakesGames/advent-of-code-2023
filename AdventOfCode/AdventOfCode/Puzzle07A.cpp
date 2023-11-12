#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle07A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle07.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle07A not yet solved!";
	}
} // namespace Puzzle07A