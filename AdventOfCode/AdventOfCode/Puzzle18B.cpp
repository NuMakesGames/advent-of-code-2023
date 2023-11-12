#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle18B
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle18.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle18B not yet solved!";
	}
} // namespace Puzzle18B