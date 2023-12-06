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

		// Solve quadratic equation to find smallest and largest winning hold times:
		//     heldMs * (time - heldMs) > distanceRecord
		//
		float a = -1.0f;
		float b = static_cast<float>(time);
		float c = static_cast<float>(-(distanceRecord + 1)); // +1 to beat the distance record
		float discriminant = std::sqrt(b * b - 4 * a * c);
		float minX = (b - discriminant) / 2.0f;
		float maxX = (b + discriminant) / 2.0f;
		int64_t wins = static_cast<int>(std::floor(maxX)) - static_cast<int>(std::ceil(minX)) + 1;

		std::cout << wins;
	}
} // namespace Puzzle06B