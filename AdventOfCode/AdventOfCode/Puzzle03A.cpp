#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace std::literals::chrono_literals;
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

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid(inputFile);

		// Render visualization, if requested
		ConsoleRenderer renderer{ grid.Width(), grid.Height(), 1 };
		if (shouldRender)
		{
			renderer.SetRestoreCursorAfterPresent(false);
			renderer.DrawGrid(grid, ConsoleForegroundColor::White);
			for (int i = 0; i < grid.size(); ++i)
			{
				char c = grid.at(i);
				if (c != '.' && !std::isdigit(c))
				{
					renderer.Draw(grid.GetCoordinatesFromIndex(i), c, ConsoleForegroundColor::IntenseMagenta);
				}
			}
			renderer.Present();
		}

		std::vector<int> numbers;
		for (int y = 0; y < grid.Height(); ++y)
		{
			int iStart = -1;
			int currentNumber = 0;
			bool hasAdjacentSymbol = false;
			for (int x = 0; x < grid.Width(); ++x)
			{
				char c = grid.at(x, y);
				bool isDigit = std::isdigit(c);

				// If the current character is a digit, continue building the current number
				if (isDigit)
				{
					if (iStart < 0)
					{
						iStart = x;
					}

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

					// Render visualization, if requested
					if (shouldRender && iStart >= 0)
					{
						for (int i = iStart; isDigit ? i <= x : i < x; ++i)
						{
							renderer.Draw(
								i,
								y,
								grid.at(i, y),
								hasAdjacentSymbol ? ConsoleForegroundColor::IntenseGreen : ConsoleForegroundColor::IntenseRed);
						}
						renderer.Present();
					}

					// Reset state for next number
					hasAdjacentSymbol = false;
					currentNumber = 0;
					iStart = -1;
				}
			}
		}

		// Restore cursor position before writing output, if rendering was enabled
		if (shouldRender)
		{
			renderer.MoveCursorBelowRenderer();
		}

		std::cout << ranges::accumulate(numbers, 0);
	}
} // namespace Puzzle03A