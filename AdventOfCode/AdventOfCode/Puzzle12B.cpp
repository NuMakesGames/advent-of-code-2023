#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle12B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle12.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle12B not yet solved!";
	}
} // namespace Puzzle12B