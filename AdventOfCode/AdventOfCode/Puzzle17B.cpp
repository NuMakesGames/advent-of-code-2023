#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle17B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle17.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle17B not yet solved!";
	}
} // namespace Puzzle17B