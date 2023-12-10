#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle10B
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

		// Replace any junk pipes with ground
		for (int i = 0; i < grid.size(); ++i)
		{
			char c = grid.at(i);
			if (c != '.' && distances.at(i) == -1)
			{
				grid.at(i) = '.';
			}
		}

		// Double the size of the grid to facilitate pathfinding between pipes
		Grid2d<char> bigGrid{ grid.Width() * 2, grid.Height() * 2 };

		// Initialize the grid with empty space
		std::ranges::fill(bigGrid, ' ');

		// Initialize the top and bottom rows with 0s
		for (int x = 0; x < bigGrid.Width(); ++x)
		{
			bigGrid.at(x, 0) = '0';
			bigGrid.at(x, bigGrid.Height() - 1) = '0';
		}

		// Initialize the left and right columns with 0s
		for (int y = 0; y < bigGrid.Height(); ++y)
		{
			bigGrid.at(0, y) = '0';
			bigGrid.at(bigGrid.Width() - 1, y) = '0';
		}

		for (int y = 0; y < grid.Height(); ++y)
		{
			for (int x = 0; x < grid.Width(); ++x)
			{
				// Set non-ground pipes to their original value
				char c = grid.at(x, y);
				if (c != '.' && distances.at(x, y) != -1)
				{
					bigGrid.at(x * 2, y * 2) = c;
					continue;
				}

				// If it's on an outer edge, we've already set it to 0
				if (x == 0 || x == grid.Width() - 1 || y == 0 || y == grid.Height() - 1)
				{
					continue;
				}

				// Replace ground with I
				bigGrid.at(x * 2, y * 2) = 'I';
			}
		}

		// Walk through pipe west/east and north/south pairs; mark any blocking connections with X
		for (int y = 0; y < grid.Height(); ++y)
		{
			for (int x = 0; x < grid.Width(); ++x)
			{
				if (x < grid.Width() - 1)
				{
					char westPipe = grid.at(x, y);
					char eastPipe = grid.at(x + 1, y);
					switch (westPipe)
					{
						case '-':
						case 'L':
						case 'F':
							switch (eastPipe)
							{
								case '-':
								case 'J':
								case '7':
									bigGrid.at(x * 2 + 1, y * 2) = 'X';
									break;
							}
							break;
					}
				}

				if (y < grid.Height() - 1)
				{
					char northPipe = grid.at(x, y);
					char southPipe = grid.at(x, y + 1);
					switch (northPipe)
					{
						case '|':
						case '7':
						case 'F':
							switch (southPipe)
							{
								case '|':
								case 'J':
								case 'L':
									bigGrid.at(x * 2, y * 2 + 1) = 'X';
									break;
							}
							break;
					}
				}
			}
		}

		// From each 0, flood any Is or empty space with 0s
		for (int y = 0; y < bigGrid.Height(); ++y)
		{
			for (int x = 0; x < bigGrid.Width(); ++x)
			{
				// Skip if not an 0
				char c = bigGrid.at(x, y);
				if (c != '0')
				{
					continue;
				}

				// Depth first flood fill
				Vector2d<int> currentPos{ x, y };
				std::stack<Vector2d<int>> toFill;
				toFill.emplace(currentPos + up);
				toFill.emplace(currentPos + left);
				toFill.emplace(currentPos + down);
				toFill.emplace(currentPos + right);
				while (!toFill.empty())
				{
					// Get next position to fill
					Vector2d<int> nextPosition = toFill.top();
					toFill.pop();

					// Skip if not in bounds
					if (!bigGrid.IsInBounds(nextPosition))
					{
						continue;
					}

					// Skip if already blocking
					char c = bigGrid.at(nextPosition);
					if (c != ' ' && c != 'I')
					{
						continue;
					}

					// Change to 0
					bigGrid.at(nextPosition) = '0';

					// Continue exploring
					toFill.emplace(nextPosition + up);
					toFill.emplace(nextPosition + left);
					toFill.emplace(nextPosition + down);
					toFill.emplace(nextPosition + right);
				}
			}
		}

		if (shouldRender)
		{
			ConsoleRenderer renderer{ bigGrid.Width(), bigGrid.Height(), 1 };
			for (int i = 0; i < bigGrid.size(); ++i)
			{
				if (bigGrid.at(i) >= 0)
				{
					ConsoleForegroundColor color = ConsoleForegroundColor::White;
					char c = bigGrid.at(i);
					if (c == 'I')
					{
						color = ConsoleForegroundColor::IntenseGreen;
					}
					else if (c == '0')
					{
						color = ConsoleForegroundColor::IntenseRed;
					}
					renderer.Draw(bigGrid.GetCoordinatesFromIndex(i), c, color);
				}
			}
			renderer.Present();
		}

		std::cout << std::ranges::count(bigGrid, 'I');
	}
} // namespace Puzzle10B