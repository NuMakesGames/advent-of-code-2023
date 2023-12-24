#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle23A
{
	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };
	constexpr std::array<Vector2d<int>, 4> directions = { up, down, left, right };

	struct SearchState
	{
		Vector2d<int> current;
		Grid2d<char> visited;
	};

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		Grid2d<char> grid = ReadAllLinesInFileAsGrid(inputFile);
		Vector2d<int> start{ 1, 0 };
		Vector2d<int> end{ grid.Width() - 2, grid.Height() - 1 };
		VerifyElseCrash(grid.at(start) == '.' && grid.at(end) == '.');
		grid.at(start) = 'S';

		// Find the longest path
		int64_t longestPath = 0;
		Grid2d<char> longestVisited;

		// Setup a stack to run DFS
		std::stack<SearchState> toExplore;
		toExplore.push({ start, grid });
		while (!toExplore.empty())
		{
			auto [current, visited] = toExplore.top();
			toExplore.pop();

			// If we've reached the end, check if this path is longer than the longest path we've found so far
			if (current == end)
			{
				int64_t visitedCount = std::ranges::count(visited, 'O');
				if (visitedCount > longestPath)
				{
					longestPath = visitedCount;
					longestVisited = visited;
				}
				continue;
			}

			// Explore all directions
			for (const Vector2d<int>& direction : directions)
			{
				Vector2d<int> next = current + direction;
				Grid2d<char> nextVisited = visited;
				if (!nextVisited.IsInBounds(next))
				{
					continue;
				}

				char c = nextVisited.at(next);
				if (c == '.')
				{
					nextVisited.at(next) = 'O';
					toExplore.push({ next, nextVisited });
					continue;
				}


				Vector2d<int> downhill = next;
				switch (c)
				{
					case '<':
						downhill = next + left;
						break;
					case '>':
						downhill = next + right;
						break;
					case '^':
						downhill = next + up;
						break;
					case 'v':
						downhill = next + down;
						break;
				}

				// If we're not going down a slope, we're done
				if (next == downhill)
				{
					continue;
				}

				// If we're going down a slope, check that the downhill position is valid
				if (!nextVisited.IsInBounds(downhill) || nextVisited.at(downhill) != '.')
				{
					continue;
				}

				nextVisited.at(next) = 'O';
				nextVisited.at(downhill) = 'O';
				toExplore.push({ downhill, nextVisited });
			}
		}

		std::cout << longestPath;
	}
} // namespace Puzzle23A