#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;
using namespace std::literals::chrono_literals;

namespace Puzzle14B
{
	void RenderGrid(const Grid2d<char>& grid, ConsoleRenderer& renderer)
	{
		for (int y = 0; y < grid.Height(); ++y)
		{
			for (int x = 0; x < grid.Width(); ++x)
			{
				switch (grid.at(x, y))
				{
					case '.':
						renderer.Draw(x, y, ' ');
						break;
					case '#':
						renderer.Draw(x, y, '#', ConsoleForegroundColor::White);
						break;
					case 'O':
						renderer.Draw(x, y, 'O', ConsoleForegroundColor::IntenseCyan);
						break;
				}
			}
		}
		renderer.Present();
	}

	void TiltNorth(Grid2d<char>& grid, bool shouldRender, ConsoleRenderer& renderer)
	{
		bool anyChange = true;
		while (anyChange)
		{
			if (shouldRender)
			{
				RenderGrid(grid, renderer);
			}

			anyChange = false;
			for (int y = 1; y < grid.Height(); ++y)
			{
				for (int x = 0; x < grid.Width(); ++x)
				{
					if (grid.at(x, y) != 'O')
					{
						continue;
					}

					if (grid.at(x, y - 1) != '.')
					{
						continue;
					}

					grid.at(x, y - 1) = 'O';
					grid.at(x, y) = '.';
					anyChange = true;
				}
			}
		}
	}

	void TiltSouth(Grid2d<char>& grid, bool shouldRender, ConsoleRenderer& renderer)
	{
		bool anyChange = true;
		while (anyChange)
		{
			if (shouldRender)
			{
				RenderGrid(grid, renderer);
			}

			anyChange = false;
			for (int y = grid.Height() - 2; y >= 0; --y)
			{
				for (int x = 0; x < grid.Width(); ++x)
				{
					if (grid.at(x, y) != 'O')
					{
						continue;
					}

					if (grid.at(x, y + 1) != '.')
					{
						continue;
					}

					grid.at(x, y + 1) = 'O';
					grid.at(x, y) = '.';
					anyChange = true;
				}
			}
		}
	}

	void TiltWest(Grid2d<char>& grid, bool shouldRender, ConsoleRenderer& renderer)
	{
		bool anyChange = true;
		while (anyChange)
		{
			if (shouldRender)
			{
				RenderGrid(grid, renderer);
			}

			anyChange = false;
			for (int x = 1; x < grid.Width(); ++x)
			{
				for (int y = 0; y < grid.Height(); ++y)
				{
					if (grid.at(x, y) != 'O')
					{
						continue;
					}

					if (grid.at(x - 1, y) != '.')
					{
						continue;
					}

					grid.at(x - 1, y) = 'O';
					grid.at(x, y) = '.';
					anyChange = true;
				}
			}
		}
	}

	void TiltEast(Grid2d<char>& grid, bool shouldRender, ConsoleRenderer& renderer)
	{
		bool anyChange = true;
		while (anyChange)
		{
			if (shouldRender)
			{
				RenderGrid(grid, renderer);
			}

			anyChange = false;
			for (int x = grid.Width() - 2; x >= 0; --x)
			{
				for (int y = 0; y < grid.Height(); ++y)
				{
					if (grid.at(x, y) != 'O')
					{
						continue;
					}

					if (grid.at(x + 1, y) != '.')
					{
						continue;
					}

					grid.at(x + 1, y) = 'O';
					grid.at(x, y) = '.';
					anyChange = true;
				}
			}
		}
	}

	int64_t ComputeLoad(const Grid2d<char>& grid)
	{
		int64_t load = 0;
		for (int y = 0; y < grid.Height(); ++y)
		{
			for (int x = 0; x < grid.Width(); ++x)
			{
				if (grid.at(x, y) == 'O')
				{
					load += grid.Height() - y;
				}
			}
		}

		return load;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto grid = ReadAllLinesInFileAsGrid(inputFile);

		ConsoleRenderer renderer{ std::max(grid.Width(), 20), grid.Height() + 1, 1 };
		renderer.SetTimeToWaitAfterPresent(1ms);

		std::vector<Grid2d<char>> grids;
		std::unordered_map<std::string, int> cycleDetector;
		bool foundCycle = false;

		constexpr int maxIterations = 1000000000;
		for (int i = 0; i < maxIterations; ++i)
		{
			if (shouldRender)
			{
				renderer.DrawString(0, grid.Height(), "Cycle " + std::to_string(i + 1), ConsoleForegroundColor::IntenseYellow);
			}

			TiltNorth(grid, shouldRender, renderer);
			TiltWest(grid, shouldRender, renderer);
			TiltSouth(grid, shouldRender, renderer);
			TiltEast(grid, shouldRender, renderer);

			if (shouldRender)
			{
				RenderGrid(grid, renderer);
			}

			// Check for cycles so that we can skip forward
			if (!foundCycle)
			{
				// Store the grid for cycle detection
				grids.push_back(grid);

				constexpr int cycleTestLength = 5;
				if (i > cycleTestLength)
				{
					// Serialize the last cycleTestLength grids into a string for cycle detection
					std::string cycleString;
					cycleString.reserve(grid.size() * cycleTestLength);
					for (int j = i - cycleTestLength + 1; j <= i; ++j)
					{
						auto& currentGrid = grids[j];
						std::ranges::for_each(currentGrid, [&cycleString](char c) { cycleString += c; });
					}

					// We've seen this configuration before; we've found a cycle.
					if (cycleDetector.contains(cycleString))
					{
						foundCycle = true;
						int cycleLength = i - cycleDetector[cycleString];

						// Skip iterations where the cycle repeats until the final set of iterations.
						i += (maxIterations - i - 1) / cycleLength * cycleLength;
						continue;
					}

					// Store this configuration if it's the first time we've seen it
					cycleDetector[cycleString] = i;
				}
			}
		}

		std::cout << ComputeLoad(grid);
	}
} // namespace Puzzle14B