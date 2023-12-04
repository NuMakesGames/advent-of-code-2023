#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle01A
{
	void PrintSolution(const std::filesystem::path& inputFile, bool /*shouldRender*/)
	{
		std::vector<int> calibrationValues;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			// Find first digit between 1 and 9
			int first = 0;
			for (char c : line)
			{
				if (c >= '1' && c <= '9')
				{
					first = c - '0';
					break;
				}
			}

			// Find last digit between 1 and 9
			int last = 0;
			for (char c : line | std::views::reverse)
			{
				if (c >= '1' && c <= '9')
				{
					last = c - '0';
					break;
				}
			}

			// Capture the two digit calibration number
			calibrationValues.push_back(first * 10 + last);
		}

		// Add all calibration values together
		std::cout << ranges::accumulate(calibrationValues, 0);
	}
} // namespace Puzzle01A