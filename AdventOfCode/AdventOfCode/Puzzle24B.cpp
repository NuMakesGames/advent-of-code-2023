#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle24B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle24.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle24B not yet solved!";
	}
} // namespace Puzzle24B