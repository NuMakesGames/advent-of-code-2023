#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(OffsetGrid2dTests, ConstructionInitializesToZero)
{
	auto grid = Utilities::OffsetGrid2d<int>{ 5, 10, 1, 2 };
	ASSERT_EQ(grid.Width(), 5);
	ASSERT_EQ(grid.Height(), 10);

	for (auto value : grid)
	{
		EXPECT_EQ(value, 0);
	}
}

TEST(OffsetGrid2dTests, CreateFromVector2ds)
{
	auto vector2ds = std::vector<Utilities::Vector2d<int>>{ { -2, -5 }, { 0, 0 }, { 1, 3 } };
	auto grid = Utilities::OffsetGrid2d<int>::CreateFromVector2ds(vector2ds);
	ASSERT_EQ(grid.Width(), 4);
	ASSERT_EQ(grid.Height(), 9);
	ASSERT_EQ(grid.OffsetX(), 2);
	ASSERT_EQ(grid.OffsetY(), 5);
}

TEST(OffsetGrid2dTests, AtCanBeUsedToSetValues)
{
	auto grid = Utilities::OffsetGrid2d<int>{ 3, 4, 1, 2 };
	ASSERT_EQ(grid.Width(), 3);
	ASSERT_EQ(grid.Height(), 4);

	for (auto y = -2; y < grid.Height() - 2; ++y)
	{
		for (auto x = -1; x < grid.Width() - 1; ++x)
		{
			grid.at(x, y) = y * 10 + x;
		}
	}

	for (auto y = -2; y < grid.Height() - 2; ++y)
	{
		for (auto x = -1; x < grid.Width() - 1; ++x)
		{
			EXPECT_EQ(grid.at(x, y), y * 10 + x);
		}
	}

	for (auto y = -2; y < grid.Height() - 2; ++y)
	{
		for (auto x = -1; x < grid.Width() - 1; ++x)
		{
			grid.at(Utilities::Vector2d<int>{ x, y }) = y * 10 + x;
		}
	}

	for (auto y = -2; y < grid.Height() - 2; ++y)
	{
		for (auto x = -1; x < grid.Width() - 1; ++x)
		{
			EXPECT_EQ(grid.at(Utilities::Vector2d<int>{ x, y }), y * 10 + x);
		}
	}
}

TEST(OffsetGrid2dTests, GetRowsReturnsExpectedRows)
{
	auto grid = Utilities::OffsetGrid2d<int>{ 5, 2, 1, 1 };
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

TEST(OffsetGrid2dTests, ComparisonOperators)
{
	{
		auto left = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
		auto right = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::OffsetGrid2d<int>{ 1, 2, 1, 2 };
		auto right = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::OffsetGrid2d<int>{ 1, 2, 0, 2 };
		auto right = Utilities::OffsetGrid2d<int>{ 1, 2, 1, 2 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::OffsetGrid2d<int>{ 1, 2, 1, 1 };
		auto right = Utilities::OffsetGrid2d<int>{ 1, 2, 1, 2 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
		auto right = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
		std::ranges::fill(right, 1);
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}
}

TEST(OffsetGrid2dTests, IndexToCoordinatesConversions)
{
	auto grid = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
	ASSERT_EQ(grid.size(), 6);

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(0);
		EXPECT_EQ(x, -1);
		EXPECT_EQ(y, -2);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 0);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(1);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, -2);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 1);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(2);
		EXPECT_EQ(x, -1);
		EXPECT_EQ(y, -1);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 2);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(3);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, -1);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 3);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(4);
		EXPECT_EQ(x, -1);
		EXPECT_EQ(y, 0);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 4);
	}

	{
		auto [x, y] = grid.GetCoordinatesFromIndex(5);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 0);

		auto i = grid.GetIndexFromCoordinates(x, y);
		EXPECT_EQ(i, 5);
	}
}

TEST(OffsetGrid2dTests, IsInBounds)
{
	auto grid = Utilities::OffsetGrid2d<int>{ 2, 3, 1, 2 };
	ASSERT_EQ(grid.size(), 6);

	for (auto y = -2; y < grid.Height() - 2; ++y)
	{
		for (auto x = -1; x < grid.Width() - 1; ++x)
		{
			EXPECT_TRUE(grid.IsInBounds(x, y));
			EXPECT_TRUE(grid.IsInBounds(Utilities::Vector2d<int>{ x, y }));
		}
	}

	EXPECT_FALSE(grid.IsInBounds(-2, 0));
	EXPECT_FALSE(grid.IsInBounds(grid.Width() - 1, 0));
	EXPECT_FALSE(grid.IsInBounds(0, -3));
	EXPECT_FALSE(grid.IsInBounds(0, grid.Height() - 2));

	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ -2, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ grid.Width() - 1, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ 0, -3 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector2d<int>{ 0, grid.Height() - 2}));
}

TEST(OffsetGrid2dTests, SwapUnderlyingBuffers)
{
	auto gridA = Utilities::OffsetGrid2d<int>{ 5, 10, 0, 0 };
	auto gridB = Utilities::OffsetGrid2d<int>{ 5, 10, 0, 0 };

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