#include "pch.h"

#include "../Utilities/Utilities.h"
#include "PuzzleSolvers.h"

using namespace Utilities;

namespace Puzzle22A
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

		// Figure out which bricks support which other bricks
		std::unordered_map<int, std::unordered_set<int>> supporting;
		std::unordered_map<int, std::unordered_set<int>> supportedBy;
		Grid3d<int> grid = SerializeBricksToGrid(bricks);
		for (const Brick& brick : bricks)
		{
			int maxZ = std::max(brick.begin.z, brick.end.z);
			if (maxZ == grid.Depth() - 1)
			{
				continue;
			}

			for (int x = brick.begin.x; x <= brick.end.x; ++x)
			{
				for (int y = brick.begin.y; y <= brick.end.y; ++y)
				{
					int id = grid.at(x, y, maxZ + 1);
					if (id != -1)
					{
						supporting[brick.id].insert(id);
						supportedBy[id].insert(brick.id);
					}
				}
			}
		}

		// Count bricks that either support no other brick or that are supported by more than 1 brick
		int64_t disintegratable = std::ranges::count_if(
			bricks,
			[&supporting, &supportedBy](const Brick& brick)
			{
				if (supporting[brick.id].size() == 0)
				{
					return true;
				}

				for (int id : supporting[brick.id])
				{
					if (supportedBy[id].size() == 1)
					{
						return false;
					}
				}
				return true;
			});

		std::cout << disintegratable;
	}
} // namespace Puzzle22A