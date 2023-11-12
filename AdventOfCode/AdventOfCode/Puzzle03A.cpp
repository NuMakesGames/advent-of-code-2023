#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle03A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle03.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle03A not yet solved!";
	}
} // namespace Puzzle03A