#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle21A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle21.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle21A not yet solved!";
	}
} // namespace Puzzle21A