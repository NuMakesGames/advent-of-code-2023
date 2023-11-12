#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle17A
{
	auto ReadInput()
	{
		auto input = ReadAllLinesInFile("Puzzle17.input");

		return input;
	}

	void PrintSolution()
	{
		auto input = ReadInput();

		std::cout << "Puzzle17A not yet solved!";
	}
} // namespace Puzzle17A