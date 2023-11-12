#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle15A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle15.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle15A not yet solved!";
	}
} // namespace Puzzle15A