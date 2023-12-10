#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(OffsetGrid3dTests, ConstructionInitializesToZero)
{
	auto grid = Utilities::OffsetGrid3d<int>{ 5, 10, 15, 1, 2, 3 };
	ASSERT_EQ(grid.Width(), 5);
	ASSERT_EQ(grid.Height(), 10);
	ASSERT_EQ(grid.Depth(), 15);

	for (auto value : grid)
	{
		EXPECT_EQ(value, 0);
	}
}

TEST(OffsetGrid3dTests, CreateFromVector3ds)
{
	auto vector3ds = std::vector<Utilities::Vector3d<int>>{ { -2, -5, -1 }, { 0, 0, 0 }, { 1, 3, 4 } };
	auto grid = Utilities::OffsetGrid3d<int>::CreateFromVector3ds(vector3ds);
	ASSERT_EQ(grid.Width(), 4);
	ASSERT_EQ(grid.Height(), 9);
	ASSERT_EQ(grid.Depth(), 6);
	ASSERT_EQ(grid.OffsetX(), 2);
	ASSERT_EQ(grid.OffsetY(), 5);
	ASSERT_EQ(grid.OffsetZ(), 1);
}

TEST(OffsetGrid3dTests, AtCanBeUsedToSetValues)
{
	auto grid = Utilities::OffsetGrid3d<int>{ 3, 4, 5, 1, 2, 3 };
	ASSERT_EQ(grid.Width(), 3);
	ASSERT_EQ(grid.Height(), 4);
	ASSERT_EQ(grid.Depth(), 5);

	for (auto z = -3; z < grid.Depth() - 3; ++z)
	{
		for (auto y = -2; y < grid.Height() - 2; ++y)
		{
			for (auto x = -1; x < grid.Width() - 1; ++x)
			{
				grid.at(x, y, z) = z * 100 + y * 10 + x;
			}
		}
	}

	for (auto z = -3; z < grid.Depth() - 3; ++z)
	{
		for (auto y = -2; y < grid.Height() - 2; ++y)
		{
			for (auto x = -1; x < grid.Width() - 1; ++x)
			{
				EXPECT_EQ(grid.at(x, y, z), z * 100 + y * 10 + x);
			}
		}
	}

	for (auto z = -3; z < grid.Depth() - 3; ++z)
	{
		for (auto y = -2; y < grid.Height() - 2; ++y)
		{
			for (auto x = -1; x < grid.Width() - 1; ++x)
			{
				grid.at(Utilities::Vector3d<int>{ x, y, z }) = z * 100 + y * 10 + x;
			}
		}
	}

	for (auto z = -3; z < grid.Depth() - 3; ++z)
	{
		for (auto y = -2; y < grid.Height() - 2; ++y)
		{
			for (auto x = -1; x < grid.Width() - 1; ++x)
			{
				EXPECT_EQ(grid.at(Utilities::Vector3d<int>{ x, y, z }), z * 100 + y * 10 + x);
			}
		}
	}
}

TEST(OffsetGrid3dTests, ComparisonOperators)
{
	{
		auto left = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		auto right = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::OffsetGrid3d<int>{ 1, 2, 3, 1, 2, 3 };
		auto right = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 0, 1, 2 };
		auto right = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		auto right = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 1 };
		EXPECT_LT(right, left);
		EXPECT_NE(left, right);
		EXPECT_GT(left, right);
	}

	{
		auto left = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		auto right = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
		std::ranges::fill(right, 1);
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}
}

TEST(OffsetGrid3dTests, IndexToCoordinatesConversions)
{
	auto grid = Utilities::OffsetGrid3d<int>{ 2, 4, 5, 1, 2, 3 };
	ASSERT_EQ(grid.size(), 40);

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(0);
		EXPECT_EQ(x, -1);
		EXPECT_EQ(y, -2);
		EXPECT_EQ(z, -3);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 0);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(1);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, -2);
		EXPECT_EQ(z, -3);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 1);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(2);
		EXPECT_EQ(x, -1);
		EXPECT_EQ(y, -1);
		EXPECT_EQ(z, -3);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 2);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(3);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, -1);
		EXPECT_EQ(z, -3);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 3);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(8);
		EXPECT_EQ(x, -1);
		EXPECT_EQ(y, -2);
		EXPECT_EQ(z, -2);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 8);
	}
}

TEST(OffsetGrid3dTests, IsInBounds)
{
	auto grid = Utilities::OffsetGrid3d<int>{ 2, 3, 4, 1, 2, 3 };
	ASSERT_EQ(grid.size(), 24);

	for (auto z = -3; z < grid.Depth() - 3; ++z)
	{
		for (auto y = -2; y < grid.Height() - 2; ++y)
		{
			for (auto x = -1; x < grid.Width() - 1; ++x)
			{
				EXPECT_TRUE(grid.IsInBounds(x, y, z));
				EXPECT_TRUE(grid.IsInBounds(Utilities::Vector3d<int>{ x, y, z }));
			}
		}
	}

	EXPECT_FALSE(grid.IsInBounds(-2, 0, 0));
	EXPECT_FALSE(grid.IsInBounds(grid.Width() - 1, 0, 0));
	EXPECT_FALSE(grid.IsInBounds(0, -3, 0));
	EXPECT_FALSE(grid.IsInBounds(0, grid.Height() - 2, 0));
	EXPECT_FALSE(grid.IsInBounds(0, 0, -4));
	EXPECT_FALSE(grid.IsInBounds(0, 0, grid.Depth() - 3));

	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ -2, 0, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ grid.Width() - 1, 0, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, -3, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, grid.Height() - 2, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, 0, -4 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, 0, grid.Depth() - 3 }));
}

TEST(OffsetGrid3dTests, SwapUnderlyingBuffers)
{
	auto gridA = Utilities::OffsetGrid3d<int>{ 5, 10, 15, 1, 2, 3 };
	auto gridB = Utilities::OffsetGrid3d<int>{ 5, 10, 15, 1, 2, 3 };

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