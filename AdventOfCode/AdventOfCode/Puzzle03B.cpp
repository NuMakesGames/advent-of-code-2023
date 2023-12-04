#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace std::literals::chrono_literals;
using namespace Utilities;

namespace Puzzle03B
{
	// Simple struct to keep track of numbers and their original position
	struct NumberAndPosition
	{
		int number = 0;

		Vector2d<int> position = { -1, -1 };
		int length = 0;
	};

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
				if (c == '*')
				{
					renderer.Draw(grid.GetCoordinatesFromIndex(i), c, ConsoleForegroundColor::IntenseYellow);
				}
			}
			renderer.Present();
		}

		std::unordered_map<Vector2d<int>, std::vector<NumberAndPosition>> gearsToAdjacentNumbers;
		for (int y = 0; y < grid.Height(); ++y)
		{
			NumberAndPosition currentNumber;
			std::unordered_set<Vector2d<int>> adjacentGears;
			for (int x = 0; x < grid.Width(); ++x)
			{
				char c = grid.at(x, y);
				bool isDigit = std::isdigit(c);

				// If the current character is a digit, continue building the current number
				if (isDigit)
				{
					if (currentNumber.position < Vector2d<int>{ 0, 0 })
					{
						currentNumber.position = { x, y };
					}

					int digit = c - '0';
					currentNumber.number *= 10;
					currentNumber.number += digit;
					currentNumber.length++;

					// Track any gears adjacent to the current digit
					adjacentGears.insert_range(GetAdjacentGears(grid, { x, y }));
				}

				// If we've reached the end of the row or if the current character is not a digit, finish current number
				if (x == grid.Width() - 1 || !isDigit)
				{
					for (const Vector2d<int>& gear : adjacentGears)
					{
						std::vector<NumberAndPosition>& adjacentNumbers = gearsToAdjacentNumbers[gear];
						adjacentNumbers.push_back(currentNumber);

						// Render visualization, if requested
						if (shouldRender)
						{
							ConsoleForegroundColor color =
								adjacentNumbers.size() == 2 ? ConsoleForegroundColor::IntenseGreen : ConsoleForegroundColor::IntenseRed;
							renderer.Draw(gear, grid.at(gear), color);
							for (const NumberAndPosition& number : adjacentNumbers)
							{
								for (int i = number.position.x; i < number.position.x + number.length; ++i)
								{
									renderer.Draw(i, number.position.y, grid.at(i, number.position.y), color);
								}
							}
							renderer.Present();
						}
					}

					// Reset state for next number
					adjacentGears.clear();
					currentNumber = NumberAndPosition{};
				}
			}
		}

		// Restore cursor position before writing output, if rendering was enabled
		if (shouldRender)
		{
			renderer.MoveCursorBelowRenderer();
		}

		// Multiply the two adjacent numbers for each gear that has exactly two adjacent numbers
		std::cout << ranges::accumulate(
			gearsToAdjacentNumbers | std::views::values,
			0,
			[](int acc, const std::vector<NumberAndPosition>& numbers)
			{ return acc + (numbers.size() == 2 ? numbers[0].number * numbers[1].number : 0); });
	}
} // namespace Puzzle03B