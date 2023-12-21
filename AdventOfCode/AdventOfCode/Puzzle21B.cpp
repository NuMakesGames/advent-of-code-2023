#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle21B
{
	struct PositionSteps
	{
		int steps = 0;
		Vector2d<int> position;

		auto operator<=>(const PositionSteps&) const = default;
	};
} // namespace Puzzle21B

IMPLEMENT_STD_HASH(Puzzle21B::PositionSteps, value.steps, value.position);

namespace Puzzle21B
{
	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };
	std::vector<Vector2d<int>> directions = { up, down, left, right };

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid(inputFile);

		// Find three points required to find the quadratic equation
		// Plug these terms into the WolframAlpha quadratic fit calculator:
		// https://www.wolframalpha.com/input?i=quadratic+fit+calculator
		constexpr bool findTerms = false;
		if (findTerms)
		{
			int firstPoint = static_cast<int>(grid.Width()) / 2;
			int secondPoint = firstPoint + static_cast<int>(grid.Width());
			int thirdPoint = secondPoint + static_cast<int>(grid.Width());

			int totalSteps = thirdPoint;
			auto startIt = std::ranges::find(grid, 'S');
			PositionSteps start{ 0, grid.GetCoordinatesFromIndex(static_cast<int>(startIt - grid.begin())) };

			std::vector<std::unordered_set<Vector2d<int>>> positionsAtSteps(totalSteps);

			std::queue<PositionSteps> toExplore;
			toExplore.push(start);
			while (!toExplore.empty())
			{
				auto [steps, position] = toExplore.front();
				toExplore.pop();

				for (const Vector2d<int>& direction : directions)
				{
					Vector2d<int> newPosition = position + direction;
					Vector2d<int> gridPosition = newPosition;
					if (!grid.IsInBounds(gridPosition))
					{
						if (gridPosition.x < 0)
						{
							gridPosition.x = (gridPosition.x % grid.Width() + grid.Width()) % grid.Width();
						}
						else if (gridPosition.x >= grid.Width())
						{
							gridPosition.x %= grid.Width();
						}

						if (gridPosition.y < 0)
						{
							gridPosition.y = (gridPosition.y % grid.Height() + grid.Height()) % grid.Height();
						}
						else if (gridPosition.y >= grid.Height())
						{
							gridPosition.y %= grid.Height();
						}
					}

					if (grid.at(gridPosition) != '#')
					{
						PositionSteps next{ steps + 1, newPosition };

						if (next.steps <= totalSteps && !positionsAtSteps[next.steps - 1].contains(next.position))
						{
							positionsAtSteps[next.steps - 1].insert(next.position);
							toExplore.push(next);
						}
					}
				}
			}

			std::cout << "Steps,Positions\n";
			std::cout << firstPoint << ',' << positionsAtSteps[firstPoint - 1].size() << "\n";
			std::cout << secondPoint << ',' << positionsAtSteps[secondPoint - 1].size() << "\n";
			std::cout << thirdPoint << ',' << positionsAtSteps[thirdPoint - 1].size() << "\n";
		}

		// Apply solved formula, in the case of my input:
		//   14696 x^2 + 14836 x + 3738
		//   (data is perfectly fit by a 2nd degree polynomial)
		constexpr int64_t desiredSteps = 26501365;
		constexpr int64_t a = 14696;
		constexpr int64_t b = 14836;
		constexpr int64_t c = 3738;
		int64_t x = (desiredSteps - (grid.Width() / 2)) / grid.Width();
		std::cout << a * x * x + b * x + c;
	}
} // namespace Puzzle21B