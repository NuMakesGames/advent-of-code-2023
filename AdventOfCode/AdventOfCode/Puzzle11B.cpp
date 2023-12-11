#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle11B
{
	auto ReadInput(const std::filesystem::path& inputFile)
	{
		auto grid = ReadAllLinesInFileAsGrid(inputFile);

		// Initialize emptyRows with all rows
		std::unordered_set<int> emptyRows;
		emptyRows.insert_range(std::views::iota(0, grid.Height()));

		// Initialize emptyColumns with all columns
		std::unordered_set<int> emptyColumns;
		emptyColumns.insert_range(std::views::iota(0, grid.Width()));

		// Find all galaxies
		std::vector<Vector2d<int>> galaxyPositions;
		for (int i = 0; i < grid.size(); ++i)
		{
			char c = grid.at(i);
			if (c != '#')
			{
				continue;
			}

			// Note the position of the galaxy
			Vector2d<int> position = grid.GetCoordinatesFromIndex(i);
			galaxyPositions.push_back(position);

			// Mark the galaxy's row and column as non-empty
			emptyColumns.erase(position.x);
			emptyRows.erase(position.y);
		}

		return std::make_tuple(galaxyPositions, emptyRows, emptyColumns);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto [galaxyPositions, emptyRows, emptyColumns] = ReadInput(inputFile);

		// Walk through each pair of galaxies and calculate the distance between them
		int64_t totalDistances = 0;
		for (int i = 0; i < galaxyPositions.size() - 1; ++i)
		{
			const Vector2d<int>& leftPosition = galaxyPositions[i];
			for (int j = i + 1; j < galaxyPositions.size(); ++j)
			{
				const Vector2d<int>& rightPosition = galaxyPositions[j];

				// Base distance is just the Manhattan distance between the galaxies' positions
				int manhattanDistance = std::abs(leftPosition.x - rightPosition.x) + std::abs(leftPosition.y - rightPosition.y);

				// Count the number of empty rows between the galaxies' positions
				int64_t rowsBetweenPositions = std::ranges::count_if(
					emptyRows,
					[&](int row)
					{ return leftPosition.y < row && row < rightPosition.y || rightPosition.y < row && row < leftPosition.y; });

				// Count the number of empty columns between the galaxies' positions
				int64_t columnsBetweenPositions = std::ranges::count_if(
					emptyColumns,
					[&](int column)
					{ return leftPosition.x < column && column < rightPosition.x || rightPosition.x < column && column < leftPosition.x; });

				// Distance of the universe expansion is the sum of the empty rows and columns between the galaxies times the amount of
				// expansion. Subtract 1 from expansion amount because 1 row/column is already accounted for in the base distance.
				constexpr int expansionAmount = 1000000 - 1;
				int64_t expansionDistance = expansionAmount * (rowsBetweenPositions + columnsBetweenPositions);

				// Add the distance between the galaxies to the total
				totalDistances += manhattanDistance + expansionDistance;
			}
		}

		std::cout << totalDistances;
	}
} // namespace Puzzle11B