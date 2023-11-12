#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle16B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle16.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle16B not yet solved!";
	}
} // namespace Puzzle16B