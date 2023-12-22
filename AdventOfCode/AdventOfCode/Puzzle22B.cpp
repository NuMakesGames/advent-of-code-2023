#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle22B
{
	struct Brick
	{
		Vector3d<int> begin;
		Vector3d<int> end;
		int id;

		auto operator<=>(const Brick&) const = default;
	};

	std::vector<Brick> ReadInput(const std::filesystem::path& inputFile)
	{
		std::vector<Brick> bricks;
		int id = 1;
		for (const std::string& line : ReadAllLinesInFile(inputFile))
		{
			std::vector<std::string> tokens = SplitString(line, "~");
			std::vector<int> start = SplitCommaSeparatedInt32s(tokens[0]);
			std::vector<int> end = SplitCommaSeparatedInt32s(tokens[1]);
			bricks.emplace_back(Vector3d<int>{ start[0], start[1], start[2] }, Vector3d<int>{ end[0], end[1], end[2] }, id++);
		}

		return bricks;
	}

	Grid3d<int> SerializeBricksToGrid(const std::vector<Brick>& bricks)
	{
		// Find the maximum X, Y, and Z coordinates for the grid size
		int maxX = 0;
		int maxY = 0;
		int maxZ = 0;
		for (const Brick& brick : bricks)
		{
			maxX = std::max(maxX, brick.begin.x);
			maxX = std::max(maxX, brick.end.x);

			maxY = std::max(maxY, brick.begin.y);
			maxY = std::max(maxY, brick.end.y);

			maxZ = std::max(maxZ, brick.begin.z);
			maxZ = std::max(maxZ, brick.end.z);
		}

		// Fill in each brick
		Grid3d<int> grid{ maxX + 1, maxY + 1, maxZ + 1 };
		std::ranges::fill(grid, -1);

		for (const Brick& brick : bricks)
		{
			for (int x = brick.begin.x; x <= brick.end.x; ++x)
			{
				for (int y = brick.begin.y; y <= brick.end.y; ++y)
				{
					for (int z = brick.begin.z; z <= brick.end.z; ++z)
					{
						grid.at(x, y, z) = brick.id;
					}
				}
			}
		}

		// Floor is flat at z = 0
		for (int x = 0; x < grid.Width(); ++x)
		{
			for (int y = 0; y < grid.Height(); ++y)
			{
				grid.at(x, y, 0) = 0;
			}
		}

		return grid;
	}

	void SettleBricks(std::vector<Brick>& bricks)
	{
		bool anyBrickMoved = true;
		while (anyBrickMoved)
		{
			anyBrickMoved = false;

			// Serialize bricks to grid to make collision checking trivial
			Grid3d<int> grid = SerializeBricksToGrid(bricks);

			// Sort bricks by lowest Z coordinate
			std::ranges::sort(
				bricks,
				[](const Brick& lhs, const Brick& rhs)
				{
					int lhsMinZ = std::min(lhs.begin.z, lhs.end.z);
					int rhsMinZ = std::min(rhs.begin.z, rhs.end.z);
					return lhsMinZ != rhsMinZ ? lhsMinZ < rhsMinZ : lhs < rhs;
				});

			for (Brick& brick : bricks)
			{
				// Nothing to do if the brick is already on the floor
				int minZ = std::min(brick.begin.z, brick.end.z);
				if (minZ == 1)
				{
					continue;
				}

				// Check if the brick can move down
				bool canMoveDown = true;
				for (int x = brick.begin.x; x <= brick.end.x && canMoveDown; ++x)
				{
					for (int y = brick.begin.y; y <= brick.end.y && canMoveDown; ++y)
					{
						if (grid.at(x, y, minZ - 1) != -1)
						{
							canMoveDown = false;
						}
					}
				}

				// Move the brick down if possible
				if (canMoveDown)
				{
					brick.begin.z -= 1;
					brick.end.z -= 1;
					anyBrickMoved = true;
				}
			}
		}
	}

	void PrintSolution(const std::filesystem::path& inputFile, bool shouldRender)
	{
		std::vector<Brick> bricks = ReadInput(inputFile);
		SettleBricks(bricks);
		std::ranges::sort(bricks);

		// Remove each brick and count the number of bricks that fall
		int64_t acc = 0;
		for (int i = 0; i < bricks.size(); ++i)
		{
			std::vector<Brick> nextBricks;
			for (int j = 0; j < bricks.size(); ++j)
			{
				// Skip the brick we're removing
				if (i == j)
				{
					continue;
				}

				nextBricks.push_back(bricks[j]);
			}

			// Settle the new set of bricks
			SettleBricks(nextBricks);
			std::ranges::sort(nextBricks);

			// Bricks that fell are the just the set difference after settling the new set
			std::vector<Brick> difference;
			std::ranges::set_difference(nextBricks, bricks, std::back_inserter(difference));
			acc += difference.size();
		}

		std::cout << acc;
	}
} // namespace Puzzle22B