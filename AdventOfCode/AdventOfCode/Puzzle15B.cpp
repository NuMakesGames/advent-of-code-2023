#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle15B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle15.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle15B not yet solved!";
	}
} // namespace Puzzle15B