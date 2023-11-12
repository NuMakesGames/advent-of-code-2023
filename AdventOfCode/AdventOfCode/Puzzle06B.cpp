#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle06B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle06.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle06B not yet solved!";
	}
} // namespace Puzzle06B