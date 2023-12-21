#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle21A
{
	struct PositionSteps
	{
		int steps = 0;
		Vector2d<int> position;

		auto operator<=>(const PositionSteps&) const = default;
	};
} // namespace Puzzle21A

IMPLEMENT_STD_HASH(Puzzle21A::PositionSteps, value.steps, value.position);

namespace Puzzle21A
{
	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };
	std::vector<Vector2d<int>> directions = { up, down, left, right };

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid(inputFile);

		constexpr int totalSteps = 64;
		auto startIt = std::ranges::find(grid, 'S');
		PositionSteps start{ 0, grid.GetCoordinatesFromIndex(static_cast<int>(startIt - grid.begin())) };

		std::unordered_set<PositionSteps> visited;
		std::unordered_set<Vector2d<int>> positionsAtMaxSteps;

		std::queue<PositionSteps> toExplore;
		toExplore.push(start);
		while (!toExplore.empty())
		{
			auto [steps, position] = toExplore.front();
			toExplore.pop();

			if (steps == totalSteps)
			{
				positionsAtMaxSteps.insert(position);
				continue;
			}

			for (const Vector2d<int>& direction : directions)
			{
				Vector2d<int> newPosition = position + direction;
				if (grid.IsInBounds(newPosition) && grid.at(newPosition) != '#')
				{
					PositionSteps next{ steps + 1, newPosition };
					if (!visited.contains(next))
					{
						visited.insert(next);
						toExplore.push(next);
					}
				}
			}
		}

		std::cout << positionsAtMaxSteps.size();
	}
} // namespace Puzzle21A