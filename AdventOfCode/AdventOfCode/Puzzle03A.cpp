#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle03A
{
	static bool IsSymbolAdjacent(const Grid2d<char>& grid, const Vector2d<int>& position)
	{
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

			// Consider any character that isn't a . or a digit to be a symbol
			char c = grid.at(checkPosition);
			if (c != '.' && !std::isdigit(c))
			{
				return true;
			}
		}

		return false;
	}

	void PrintSolution()
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid("Puzzle03.input");

		std::vector<int> numbers;
		for (int y = 0; y < grid.Height(); ++y)
		{
			int currentNumber = 0;
			bool hasAdjacentSymbol = false;
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

					// Check for any symbols adjacent to the current digit, if one not already found
					hasAdjacentSymbol = hasAdjacentSymbol || IsSymbolAdjacent(grid, { x, y });
				}

				// If we've reached the end of the row or if the current character is not a digit, finish current number
				if (x == grid.Width() - 1 || !isDigit)
				{
					// If we found a symbol while parsing the number, keep it
					if (hasAdjacentSymbol)
					{
						numbers.push_back(currentNumber);
					}

					// Reset state for next number
					hasAdjacentSymbol = false;
					currentNumber = 0;
				}
			}
		}

		std::cout << ranges::accumulate(numbers, 0);
	}
} // namespace Puzzle03A