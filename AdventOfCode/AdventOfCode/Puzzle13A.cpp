#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle13A
{
	auto ReadInput(const std::filesystem::path& inputFile)
	{
		// Read each set of input lines into a grid
		std::vector<Grid2d<char>> grids = ReadAllLinesInFile(inputFile) | std::views::split("")
		                                | std::views::transform(
											  [](const auto& subrange)
											  {
												  std::vector<std::string> lines = subrange | std::ranges::to<std::vector>();
												  int width = static_cast<int>(lines[0].size());
												  int height = static_cast<int>(lines.size());
												  auto grid = Grid2d<char>{ width, height };
												  for (int y = 0; y < grid.Height(); ++y)
												  {
													  const auto& line = lines[y];
													  for (int x = 0; x < grid.Width(); ++x)
													  {
														  grid.at(x, y) = line[x];
													  }
												  }

												  return grid;
											  })
		                                | std::ranges::to<std::vector>();
		return grids;
	}

	// Look for symmetry on a vertical line
	std::pair<bool, int> FindColumnReflection(const Grid2d<char>& grid)
	{
		for (int x = 1; x < grid.Width(); ++x)
		{
			int leftX = x - 1;
			int rightX = x;
			int size = std::min(leftX - 0 + 1, grid.Width() - rightX);

			bool foundReflection = true;
			for (int y = 0; y < grid.Height(); ++y)
			{
				for (int i = 0; i < size; ++i)
				{
					if (grid.at(leftX - i, y) != grid.at(rightX + i, y))
					{
						foundReflection = false;
						break;
					}
				}
			}

			if (foundReflection)
			{
				return std::make_pair(true, x);
			}
		}

		return std::make_pair(false, 0);
	}

	// Look for symmetry on a horizontal line
	std::pair<bool, int> FindRowReflection(const Grid2d<char>& grid)
	{
		for (int y = 1; y < grid.Height(); ++y)
		{
			int leftY = y - 1;
			int rightY = y;
			int size = std::min(leftY - 0 + 1, grid.Height() - rightY);

			bool foundReflection = true;
			for (int x = 0; x < grid.Width(); ++x)
			{
				for (int i = 0; i < size; ++i)
				{
					if (grid.at(x, leftY - i) != grid.at(x, rightY + i))
					{
						foundReflection = false;
						break;
					}
				}
			}

			if (foundReflection)
			{
				return std::make_pair(true, y);
			}
		}

		return std::make_pair(false, 0);
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto grids = ReadInput(inputFile);
		int acc = 0;
		for (const Grid2d<char>& grid : grids)
		{
			auto [foundColReflection, numCols] = FindColumnReflection(grid);
			if (foundColReflection)
			{
				acc += numCols;
				continue;
			}

			auto [foundRowReflection, numRows] = FindRowReflection(grid);
			if (foundRowReflection)
			{
				acc += 100 * numRows;
			}
		}

		std::cout << acc;
	}
} // namespace Puzzle13A