#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle03B
{
	static std::unordered_set<Vector2d<int>> GetAdjacentGears(const Grid2d<char>& grid, const Vector2d<int>& position)
	{
		std::unordered_set<Vector2d<int>> adjacentGears;
		static const std::vector<Vector2d<int>> adjacentOffsets = { { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 },
			                                                        { 1, 0 },   { -1, 1 }, { 0, 1 },  { 1, 1 } };
		for (const Vector2d<int>& offset : adjacentOffsets)
		{
			Vector2d<int> checkPosition = position + offset;

			// Bail if the adjacent position is out-of-bounds
			if (checkPosition.x < 0 || checkPosition.x >= grid.Width() || checkPosition.y < 0 || checkPosition.y >= grid.Height())
			{
				continue;
			}

			// Ignore non-gear symbols
			char c = grid.at(checkPosition);
			if (c == '*')
			{
				adjacentGears.insert(checkPosition);
			}
		}

		return adjacentGears;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool /*shouldRender*/)
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid(inputFile);

		std::unordered_map<Vector2d<int>, std::vector<int>> gearsToAdjacentNumbers;
		for (int y = 0; y < grid.Height(); ++y)
		{
			int currentNumber = 0;
			std::unordered_set<Vector2d<int>> adjacentGears;
			for (int x = 0; x < grid.Width(); ++x)
			{
				char c = grid.at(x, y);
				bool isDigit = std::isdigit(c);

				// If the current character is a digit, continue building the current number
				if (isDigit)
				{
					int digit = c - '0';
					currentNumber *= 10;
					currentNumber += digit;

					// Track any gears adjacent to the current digit
					adjacentGears.insert_range(GetAdjacentGears(grid, { x, y }));
				}

				// If we've reached the end of the row or if the current character is not a digit, finish current number
				if (x == grid.Width() - 1 || !isDigit)
				{
					for (const Vector2d<int>& gear : adjacentGears)
					{
						gearsToAdjacentNumbers[gear].push_back(currentNumber);
					}

					// Reset state for next number
					adjacentGears.clear();
					currentNumber = 0;
				}
			}
		}

		// Multiply the two adjacent numbers for each gear that has exactly two adjacent numbers
		std::cout << ranges::accumulate(
			gearsToAdjacentNumbers | std::views::values,
			0,
			[](int acc, const std::vector<int>& numbers) { return acc + (numbers.size() == 2 ? numbers[0] * numbers[1] : 0); });
	}
} // namespace Puzzle03B