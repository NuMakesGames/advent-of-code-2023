#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(Grid3dTests, ConstructionInitializesToZero)
{
	auto grid = Utilities::Grid3d<int>{ 2, 3, 4 };
	ASSERT_EQ(grid.Width(), 2);
	ASSERT_EQ(grid.Height(), 3);
	ASSERT_EQ(grid.Depth(), 4);

	for (auto value : grid)
	{
		EXPECT_EQ(value, 0);
	}
}

TEST(Grid3dTests, AtCanBeUsedToSetValues)
{
	auto grid = Utilities::Grid3d<int>{ 3, 4, 5 };
	ASSERT_EQ(grid.Width(), 3);
	ASSERT_EQ(grid.Height(), 4);
	ASSERT_EQ(grid.Depth(), 5);

	for (auto z = 0; z < grid.Depth(); ++z)
	{
		for (auto y = 0; y < grid.Height(); ++y)
		{
			for (auto x = 0; x < grid.Width(); ++x)
			{
				grid.at(x, y, z) = z * 100 + y * 10 + x;
			}
		}
	}

	for (auto z = 0; z < grid.Depth(); ++z)
	{
		for (auto y = 0; y < grid.Height(); ++y)
		{
			for (auto x = 0; x < grid.Width(); ++x)
			{
				EXPECT_EQ(grid.at(x, y, z), z * 100 + y * 10 + x);
			}
		}
	}

	for (auto z = 0; z < grid.Depth(); ++z)
	{
		for (auto y = 0; y < grid.Height(); ++y)
		{
			for (auto x = 0; x < grid.Width(); ++x)
			{
				grid.at(Utilities::Vector3d<int>{ x, y, z }) = z * 100 + y * 10 + x;
			}
		}
	}

	for (auto z = 0; z < grid.Depth(); ++z)
	{
		for (auto y = 0; y < grid.Height(); ++y)
		{
			for (auto x = 0; x < grid.Width(); ++x)
			{
				EXPECT_EQ(grid.at(Utilities::Vector3d<int>{ x, y, z }), z * 100 + y * 10 + x);
			}
		}
	}
}

TEST(Grid3dTests, ComparisonOperators)
{
	{
		auto left = Utilities::Grid3d<int>{ 2, 3, 4 };
		auto right = Utilities::Grid3d<int>{ 2, 3, 4 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::Grid3d<int>{ 1, 2, 3 };
		auto right = Utilities::Grid3d<int>{ 2, 3, 4 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::Grid3d<int>{ 2, 3, 4 };
		auto right = Utilities::Grid3d<int>{ 2, 3, 4 };
		std::ranges::fill(right, 1);
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}
}

TEST(Grid3dTests, IndexToCoordinatesConversions)
{
	auto grid = Utilities::Grid3d<int>{ 2, 3, 2 };
	ASSERT_EQ(grid.size(), 12);

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(0);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 0);
		EXPECT_EQ(z, 0);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 0);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(1);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 0);
		EXPECT_EQ(z, 0);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 1);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(2);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 1);
		EXPECT_EQ(z, 0);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 2);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(3);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 1);
		EXPECT_EQ(z, 0);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 3);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(4);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 2);
		EXPECT_EQ(z, 0);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 4);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(5);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 2);
		EXPECT_EQ(z, 0);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 5);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(6);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 0);
		EXPECT_EQ(z, 1);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 6);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(7);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 0);
		EXPECT_EQ(z, 1);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 7);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(8);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 1);
		EXPECT_EQ(z, 1);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 8);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(9);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 1);
		EXPECT_EQ(z, 1);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 9);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(10);
		EXPECT_EQ(x, 0);
		EXPECT_EQ(y, 2);
		EXPECT_EQ(z, 1);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 10);
	}

	{
		auto [x, y, z] = grid.GetCoordinatesFromIndex(11);
		EXPECT_EQ(x, 1);
		EXPECT_EQ(y, 2);
		EXPECT_EQ(z, 1);

		auto i = grid.GetIndexFromCoordinates(x, y, z);
		EXPECT_EQ(i, 11);
	}
}

TEST(Grid3dTests, IsInBounds)
{
	auto grid = Utilities::Grid3d<int>{ 2, 3, 4 };
	ASSERT_EQ(grid.size(), 24);

	for (auto z = 0; z < grid.Depth(); ++z)
	{
		for (auto y = 0; y < grid.Height(); ++y)
		{
			for (auto x = 0; x < grid.Width(); ++x)
			{
				EXPECT_TRUE(grid.IsInBounds(x, y, z));
				EXPECT_TRUE(grid.IsInBounds(Utilities::Vector3d<int>{ x, y, z }));
			}
		}
	}

	EXPECT_FALSE(grid.IsInBounds(-1, 0, 0));
	EXPECT_FALSE(grid.IsInBounds(grid.Width(), 0, 0));
	EXPECT_FALSE(grid.IsInBounds(0, -1, 0));
	EXPECT_FALSE(grid.IsInBounds(0, grid.Height(), 0));
	EXPECT_FALSE(grid.IsInBounds(0, 0, -1));
	EXPECT_FALSE(grid.IsInBounds(0, 0, grid.Depth()));

	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ -1, 0, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ grid.Width(), 0, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, -1, 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, grid.Height(), 0 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, 0, -1 }));
	EXPECT_FALSE(grid.IsInBounds(Utilities::Vector3d<int>{ 0, 0, grid.Depth() }));
}

TEST(Grid3dTests, SwapUnderlyingBuffers)
{
						auto gridA = Utilities::Grid3d<int>{ 5, 10, 15 };
						auto gridB = Utilities::Grid3d<int>{ 5, 10, 15 };

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

TEST(Grid3dTests, ResizePreservesElements)
{
						auto grid = Utilities::Grid3d<int>{ 5, 5, 5 };
						std::ranges::fill(grid, 1);

						grid.resize(7, 10, 13);
						EXPECT_EQ(grid.Width(), 7);
						EXPECT_EQ(grid.Height(), 10);
						EXPECT_EQ(grid.Depth(), 13);
						for (int z = 0; z < grid.Depth(); ++z)
						{
							for (int y = 0; y < grid.Height(); ++y)
							{
								for (int x = 0; x < grid.Width(); ++x)
								{
									EXPECT_EQ(grid.at(x, y, z), x >= 5 || y >= 5 || z >= 5 ? 0 : 1);
								}
							}
						}

						grid.resize(2, 4, 5);
						EXPECT_EQ(grid.Width(), 2);
						EXPECT_EQ(grid.Height(), 4);
						EXPECT_EQ(grid.Depth(), 5);
						for (auto value : grid)
						{
							EXPECT_EQ(value, 1);
						}
}
