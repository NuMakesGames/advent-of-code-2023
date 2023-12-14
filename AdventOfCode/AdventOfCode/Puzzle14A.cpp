#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle14A
{
	void TiltNorth(Grid2d<char>& grid)
	{
		bool anyChange = true;
		while (anyChange)
		{
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
		TiltNorth(grid);
		std::cout << ComputeLoad(grid);
	}
} // namespace Puzzle14A