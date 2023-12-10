#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(Grid2dTests, ConstructionInitializesToZero)
{
	auto grid = Utilities::Grid2d<int>{ 5, 10 };
	ASSERT_EQ(grid.Width(), 5);
	ASSERT_EQ(grid.Height(), 10);

	for (auto value : grid)
	{
		EXPECT_EQ(value, 0);
	}
}

TEST(Grid2dTests, AtCanBeUsedToSetValues)
{
	auto grid = Utilities::Grid2d<int>{ 3, 4 };
	ASSERT_EQ(grid.Width(), 3);
	ASSERT_EQ(grid.Height(), 4);

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			grid.at(x, y) = y * 10 + x;
		}
	}

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			EXPECT_EQ(grid.at(x, y), y * 10 + x);
		}
	}

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			grid.at(Utilities::Vector2d<int>{ x, y }) = y * 10 + x;
		}
	}

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			EXPECT_EQ(grid.at(Utilities::Vector2d<int>{ x, y }), y * 10 + x);
		}
	}
}

TEST(Grid2dTests, GetRowsReturnsExpectedRows)
{
	auto grid = Utilities::Grid2d<int>{ 5, 2 };
	ASSERT_EQ(grid.Width(), 5);
	ASSERT_EQ(grid.Height(), 2);

	auto y = 0;
	for (const auto& row : grid.GetRows())
	{
		auto x = 0;
		for (auto& cell : row)
		{
			cell = y++ * 10 + x++;
		}
	}

	y = 0;
	for (const auto& row : grid.GetRows())
	{
		auto currentRow = std::ranges::to<std::vector>(row);
		ASSERT_LT(y++, 2);
		ASSERT_EQ(currentRow.size(), 5);
		if (y == 0)
		{
			EXPECT_EQ(currentRow[0], 10);
			EXPECT_EQ(currentRow[1], 11);
			EXPECT_EQ(currentRow[2], 12);
			EXPECT_EQ(currentRow[3], 13);
			EXPECT_EQ(currentRow[4], 14);
		}
		else if (y == 0)
		{
			EXPECT_EQ(currentRow[0], 20);
			EXPECT_EQ(currentRow[1], 21);
			EXPECT_EQ(currentRow[2], 22);
			EXPECT_EQ(currentRow[3], 23);
			EXPECT_EQ(currentRow[4], 24);
		}
	}
}

TEST(Grid2dTests, ComparisonOperators)
{
	{
		auto left = Utilities::Grid2d<int>{ 2, 3 };
		auto right = Utilities::Grid2d<int>{ 2, 3 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::Grid2d<int>{ 1, 2 };
		auto right = Utilities::Grid2d<int>{ 2, 3 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::Grid2d<int>{ 2, 3 };
		auto right = Utilities::Grid2d<int>{ 2, 3 };
		std::ranges::fill(right, 1);
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}
}

TEST(Grid2dTests, IndexToCoordinatesConversions)
{
	auto grid = Utilities::Grid2d<int>{ 2, 3 };
	ASSERT_EQ(grid.size(), 6);

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(0);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 0);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 0);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(1);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 0);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 1);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(2);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 1);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 2);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(3);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 1);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 3);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(4);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 2);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 4);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(5);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 2);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 5);
	}
}

TEST(Grid2dTests, IsInBounds)
{
	auto grid = Utilities::Grid2d<int>{ 2, 3 };
	ASSERT_EQ(grid.size(), 6);

	for (auto y = 0; y < grid.Height(); ++y)
	{
		for (auto x = 0; x < grid.Width(); ++x)
		{
			EXPECT_TRUE(grid.IsInBounds(x, y));
			EXPECT_TRUE(grid.IsInBounds(Utilities::Vector2d<int>{ x, y }));
		}
	}

	EXPECT_FALSE(grid.IsInBounds(-1, 0));
	EXPECT_FALSE(grid.IsInBounds(grid.Width(), 0));
	EXPECT_FALSE(grid.IsInBounds(0, -1));
	EXPECT_FALSE(grid.IsInBounds(0, grid.Height()));

	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ -1, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ grid.Width(), 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ 0, -1 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ 0, grid.Height() }));
}

TEST(Grid2dTests, SwapUnderlyingBuffers)
{
	auto gridA = Utilities::Grid2d<int>{ 5, 10 };
	auto gridB = Utilities::Grid2d<int>{ 5, 10 };

	std::ranges::fill(gridA, 1);
	std::ranges::fill(gridB, 2);

	gridA.swap(gridB);

	for (auto value : gridA)
	{
		EXPECT_EQ(value, 2);
	}

	for (auto value : gridB)
	{
		EXPECT_EQ(value, 1);
	}
}

TEST(Grid2dTests, ResizePreservesElements)
{
	auto grid = Utilities::Grid2d<int>{ 5, 5 };
	std::ranges::fill(grid, 1);

	grid.resize(7, 10);
	EXPECT_EQ(grid.Width(), 7);
	EXPECT_EQ(grid.Height(), 10);
	for (int y = 0; y < grid.Height(); ++y)
	{
		for (int x = 0; x < grid.Width(); ++x)
		{
			EXPECT_EQ(grid.at(x, y), x >= 5 || y >= 5 ? 0 : 1);
		}
	}

	grid.resize(2, 4);
	EXPECT_EQ(grid.Width(), 2);
	EXPECT_EQ(grid.Height(), 4);
	for (auto value : grid)
	{
		EXPECT_EQ(value, 1);
	}
}