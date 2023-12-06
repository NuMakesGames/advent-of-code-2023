#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle06B
{
	auto ReadInput(const std::filesystem::path& inputFile)
	{
		auto input = ReadAllLinesInFile(inputFile);

		// Parse time
		int64_t time = 0;
		for (char c : input[0])
		{
			if (std::isdigit(c))
			{
				time *= 10;
				time += c - '0';
			}
		}

		// Parse distance
		int64_t distance = 0;
		for (char c : input[1])
		{
			if (std::isdigit(c))
			{
				distance *= 10;
				distance += c - '0';
			}
		}

		return std::make_pair(time, distance);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [time, distanceRecord] = ReadInput(inputFile);
		int64_t wins = 0;
		for (int64_t heldMs = 1; heldMs <= time; ++heldMs)
		{
			int64_t velocity = heldMs;
			int64_t distance = velocity * (time - heldMs);
			if (distance > distanceRecord)
			{
				++wins;
			}
		}

		std::cout << wins;
	}
} // namespace Puzzle06B