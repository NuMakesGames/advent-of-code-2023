#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle10A
{
	constexpr Vector2d<int> up = { 0, -1 };
	constexpr Vector2d<int> down = { 0, 1 };
	constexpr Vector2d<int> left = { -1, 0 };
	constexpr Vector2d<int> right = { 1, 0 };

	// Retrieves the connected pipes for the pipe at the specified position
	std::vector<Vector2d<int>> GetConnectedPipes(Grid2d<char>& grid, const Vector2d<int>& position)
	{
		std::array<Vector2d<int>, 2> candidatePipes;
		char c = grid.at(position);
		switch (c)
		{
			case '|':
				candidatePipes[0] = position + up;
				candidatePipes[1] = position + down;
				break;
			case '-':
				candidatePipes[0] = position + left;
				candidatePipes[1] = position + right;
				break;
			case 'L':
				candidatePipes[0] = position + up;
				candidatePipes[1] = position + right;
				break;
			case 'J':
				candidatePipes[0] = position + up;
				candidatePipes[1] = position + left;
				break;
			case '7':
				candidatePipes[0] = position + down;
				candidatePipes[1] = position + left;
				break;
			case 'F':
				candidatePipes[0] = position + down;
				candidatePipes[1] = position + right;
				break;
		}

		std::vector<Vector2d<int>> connectedPipes;
		for (const Vector2d<int>& candidatePipe : candidatePipes)
		{
			if (grid.IsInBounds(candidatePipe))
			{
				connectedPipes.push_back(candidatePipe);
			}
		}

		return connectedPipes;
	}

	// Finds the starting pipe's position and type, and updates grid to have the correct starting pipe type
	Vector2d<int> GetStartingPipe(Grid2d<char>& grid)
	{
		// Find the starting position
		auto startIt = std::ranges::find(grid, 'S');
		Vector2d<int> startPosition = grid.GetCoordinatesFromIndex(static_cast<int>(startIt - grid.begin()));

		// Try each possible starting pipe until we find a loop
		for (char pipeType : { '|', '-', 'L', 'J', '7', 'F' })
		{
			// Swap the pipe type at the starting position
			grid.at(startPosition) = pipeType;

			// Determine what pipes must be connected to the starting pipe
			std::vector<Vector2d<int>> connectedPipes = GetConnectedPipes(grid, startPosition);

			// If the required connecting pipes would push us out-of-bounds, skip to next starting pipe
			bool areConnectedPipesValid = connectedPipes.size() == 2;
			if (!areConnectedPipesValid)
			{
				continue;
			}

			// Check whether each connected pipe has a valid type for the connection
			for (const Vector2d<int>& connectedPipe : connectedPipes)
			{
				char c = grid.at(connectedPipe);
				switch (c)
				{
					case '|':
						if (connectedPipe != startPosition + down && connectedPipe != startPosition + up)
						{
							areConnectedPipesValid = false;
						}
						break;
					case '-':
						if (connectedPipe != startPosition + left && connectedPipe != startPosition + right)
						{
							areConnectedPipesValid = false;
						}
						break;
					case 'L':
						if (connectedPipe != startPosition + down && connectedPipe != startPosition + left)
						{
							areConnectedPipesValid = false;
						}
						break;
					case 'J':
						if (connectedPipe != startPosition + down && connectedPipe != startPosition + right)
						{
							areConnectedPipesValid = false;
						}
						break;
					case '7':
						if (connectedPipe != startPosition + up && connectedPipe != startPosition + right)
						{
							areConnectedPipesValid = false;
						}
						break;
					case 'F':
						if (connectedPipe != startPosition + up && connectedPipe != startPosition + left)
						{
							areConnectedPipesValid = false;
						}
						break;
					case '.':
						areConnectedPipesValid = false;
						break;
				}
			}

			// Found the valid starting pipe type
			if (areConnectedPipesValid)
			{
				return startPosition;
			}
		}

		VerifyNotReached();
		return Vector2d<int>{};
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto grid = ReadAllLinesInFileAsGrid(inputFile);

		// Determining start position and its pipe type
		Vector2d<int> startPosition = GetStartingPipe(grid);

		// Breadth-first-search to find the distance to each pipe from the starting pipe
		Grid2d<int> distances{ grid.Width(), grid.Height() };
		std::ranges::fill(distances, -1);
		distances.at(startPosition) = 0;

		// Setup initial exploration paths
		std::queue<std::pair<Vector2d<int>, Vector2d<int>>> toExplore;
		for (const Vector2d<int>& connectedPipe : GetConnectedPipes(grid, startPosition))
		{
			toExplore.emplace(startPosition, connectedPipe);
		}

		// Explore the full loop
		while (!toExplore.empty())
		{
			auto [from, to] = toExplore.front();
			toExplore.pop();

			if (distances.at(to) != -1)
			{
				continue;
			}

			distances.at(to) = distances.at(from) + 1;
			for (const Vector2d<int>& connectedPipe : GetConnectedPipes(grid, to))
			{
				toExplore.emplace(to, connectedPipe);
			}
		}

		std::cout << std::ranges::max(distances);
	}
} // namespace Puzzle10A