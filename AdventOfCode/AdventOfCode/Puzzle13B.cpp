#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle13B
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

	// Count the number of changes required for symmetry on each vertical line
	std::vector<int> CountRequiredSmudgesForColumnReflection(const Grid2d<char>& grid)
	{
		std::vector<int> result(grid.Width(), 0);

		for (int x = 1; x < grid.Width(); ++x)
		{
			int leftX = x - 1;
			int rightX = x;
			int size = std::min(leftX - 0 + 1, grid.Width() - rightX);

			for (int y = 0; y < grid.Height(); ++y)
			{
				for (int i = 0; i < size; ++i)
				{
					if (grid.at(leftX - i, y) != grid.at(rightX + i, y))
					{
						++result[x];
					}
				}
			}
		}

		return result;
	}

	// Count the number of changes required for symmetry on each horizontal line
	std::vector<int> CountRequiredSmudgesForRowSymmetry(const Grid2d<char>& grid)
	{
		std::vector<int> result(grid.Height(), 0);

		for (int y = 1; y < grid.Height(); ++y)
		{
			int leftY = y - 1;
			int rightY = y;
			int size = std::min(leftY - 0 + 1, grid.Height() - rightY);

			for (int x = 0; x < grid.Width(); ++x)
			{
				for (int i = 0; i < size; ++i)
				{
					if (grid.at(x, leftY - i) != grid.at(x, rightY + i))
					{
						++result[y];
					}
				}
			}
		}

		return result;
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		auto grids = ReadInput(inputFile);
		int acc = 0;
		for (const Grid2d<char>& grid : grids)
		{
			bool foundSmudge = false;
			std::vector<int> asymmetricalColumns = CountRequiredSmudgesForColumnReflection(grid);
			for (int x = 0; x < asymmetricalColumns.size(); ++x)
			{
				if (asymmetricalColumns[x] == 1)
				{
					acc += x;
					foundSmudge = true;
				}
			}

			if (foundSmudge)
			{
				continue;
			}

			std::vector<int> asymmetricalRows = CountRequiredSmudgesForRowSymmetry(grid);
			for (int y = 0; y < asymmetricalRows.size(); ++y)
			{
				if (asymmetricalRows[y] == 1)
				{
					acc += 100 * y;
					foundSmudge = true;
				}
			}

			VerifyElseCrash(foundSmudge);
		}

		std::cout << acc;
	}
} // namespace Puzzle13B