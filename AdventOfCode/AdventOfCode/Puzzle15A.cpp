#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle15A
{
	std::vector<std::string> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<std::string> input = ReadAllLinesInFile(inputFile);
		return SplitString(input[0], ",");
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		int acc = 0;
		for (const std::string& str : ReadInput(inputFile))
		{
			int value = 0;
			for (char c : str)
			{
				value += static_cast<int>(c);
				value *= 17;
				value %= 256;
			}
			acc += value;
		}
		std::cout << acc;
	}
} // namespace Puzzle15A