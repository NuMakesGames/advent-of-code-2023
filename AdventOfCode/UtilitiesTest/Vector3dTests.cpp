#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(Vector3dTests, ConstructionInitializesToZero)
{
	auto vector = Utilities::Vector3d<int>{};
	ASSERT_EQ(vector.x, 0);
	ASSERT_EQ(vector.y, 0);
	ASSERT_EQ(vector.z, 0);
}

TEST(Vector3dTests, AdditionOperator)
{
	auto left = Utilities::Vector3d<int>{ 1, 2, 3 };
	auto right = Utilities::Vector3d<int>{ 4, 5, 6 };
	auto sum = left + right;
	ASSERT_EQ(sum.x, 5);
	ASSERT_EQ(sum.y, 7);
	ASSERT_EQ(sum.z, 9);
}

TEST(Vector3dTests, SubtractionOperator)
{
	auto left = Utilities::Vector3d<int>{ 1, 2, 3 };
	auto right = Utilities::Vector3d<int>{ 4, 5, 6 };
	auto difference = left - right;
	ASSERT_EQ(difference.x, -3);
	ASSERT_EQ(difference.y, -3);
	ASSERT_EQ(difference.z, -3);
}

TEST(Vector3dTests, MultiplicationOperator)
{
	auto vec = Utilities::Vector3d<int>{ 1, 2, 3 } * 5;
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 10);
	ASSERT_EQ(vec.z, 15);
}

TEST(Vector3dTests, AdditionAssignmentOperator)
{
	auto vec = Utilities::Vector3d<int>{ 1, 2, 3 };
	vec += Utilities::Vector3d<int>{ 4, 5, 6 };
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 7);
	ASSERT_EQ(vec.z, 9);
}

TEST(Vector3dTests, SubtractionAssignmentOperator)
{
	auto vec = Utilities::Vector3d<int>{ 1, 2, 3 };
	vec -= Utilities::Vector3d<int>{ 4, 5, 6 };
	ASSERT_EQ(vec.x, -3);
	ASSERT_EQ(vec.y, -3);
	ASSERT_EQ(vec.z, -3);
}

TEST(Vector3dTests, MultiplicationAssignmentOperator)
{
	auto vec = Utilities::Vector3d<int>{ 1, 2, 3 };
	vec *= 5;
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 10);
	ASSERT_EQ(vec.z, 15);
}

TEST(Vector3dTests, ComparisonOperators)
{
	{
		auto left = Utilities::Vector3d<int>{ 2, 3, 4 };
		auto right = Utilities::Vector3d<int>{ 2, 3, 4 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::Vector3d<int>{ 1, 2, 3 };
		auto right = Utilities::Vector3d<int>{ 2, 3, 4 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::Vector3d<int>{ 2, 3, 4 };
		auto right = Utilities::Vector3d<int>{ 1, 4, 5 };
		EXPECT_LT(right, left);
		EXPECT_NE(left, right);
		EXPECT_GT(left, right);
	}
}

TEST(Vector3dTests, Hashing)
{
	{
		auto hash = std::hash<Utilities::Vector3d<int>>();
		auto left = hash(Utilities::Vector3d<int>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int>{ 2, 3, 4 });
		EXPECT_EQ(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int>>();
		auto left = hash(Utilities::Vector3d<int>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int>{ 2, 3, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int>>();
		auto left = hash(Utilities::Vector3d<int>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int>{ 2, 5, 4 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int>>();
		auto left = hash(Utilities::Vector3d<int>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int>{ 5, 3, 4 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int64_t>>();
		auto left = hash(Utilities::Vector3d<int64_t>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int64_t>{ 2, 3, 4 });
		EXPECT_EQ(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int64_t>>();
		auto left = hash(Utilities::Vector3d<int64_t>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int64_t>{ 2, 3, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int64_t>>();
		auto left = hash(Utilities::Vector3d<int64_t>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int64_t>{ 2, 5, 4 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector3d<int64_t>>();
		auto left = hash(Utilities::Vector3d<int64_t>{ 2, 3, 4 });
		auto right = hash(Utilities::Vector3d<int64_t>{ 5, 3, 4 });
		EXPECT_NE(left, right);
	}
}

TEST(Vector3dTests, ToString)
{
	auto vec = Utilities::Vector3d<int>{ 1, 2, 3 };
	EXPECT_EQ(vec.ToString(), "(1, 2, 3)");
}