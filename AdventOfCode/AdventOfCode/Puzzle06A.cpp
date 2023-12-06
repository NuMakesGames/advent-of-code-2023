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

		// Multiply number of wins for each race
		int acc = 1;
		for (int i = 0; i < times.size(); ++i)
		{
			int time = times[i];
			int distanceRecord = distanceRecords[i];

			// Solve quadratic equation to find smallest and largest winning hold times:
			//     heldMs * (time - heldMs) > distanceRecord
			//
			float a = -1.0f;
			float b = static_cast<float>(time);
			float c = static_cast<float>(-(distanceRecord + 1)); // +1 to beat the distance record
			float discriminant = std::sqrt(b * b - 4 * a * c);
			float minX = (b - discriminant) / 2.0f;
			float maxX = (b + discriminant) / 2.0f;
			acc *= static_cast<int>(std::floor(maxX)) - static_cast<int>(std::ceil(minX)) + 1;
		}

		std::cout << acc;
	}
} // namespace Puzzle06A