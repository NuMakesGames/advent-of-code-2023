#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle11B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle11.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle11B not yet solved!";
	}
} // namespace Puzzle11B