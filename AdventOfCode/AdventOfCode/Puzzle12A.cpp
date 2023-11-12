#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle12A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle12.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle12A not yet solved!";
	}
} // namespace Puzzle12A