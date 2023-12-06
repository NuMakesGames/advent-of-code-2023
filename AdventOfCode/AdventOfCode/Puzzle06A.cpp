#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle06A
{
	auto ReadInput(const std::filesystem::path& inputFile)
	{
		auto input = ReadAllLinesInFile(inputFile);
		return std::make_pair(ExtractInt32s(input[0]), ExtractInt32s(input[1]));
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [times, distanceRecords] = ReadInput(inputFile);

		// Count wins for each race
		std::vector<int> wins(times.size(), 0);
		for (int i = 0; i < times.size(); ++i)
		{
			int time = times[i];
			int distanceRecord = distanceRecords[i];

			// Test each valid charging time for a win
			for (int heldMs = 1; heldMs <= time; ++heldMs)
			{
				int velocity = heldMs;
				int distance = velocity * (time - heldMs);
				if (distance > distanceRecord)
				{
					++wins[i];
				}
			}
		}

		// Multiply win counts together
		std::cout << ranges::accumulate(wins, 1, [](int acc, int val) { return acc * val; });
	}
} // namespace Puzzle06A