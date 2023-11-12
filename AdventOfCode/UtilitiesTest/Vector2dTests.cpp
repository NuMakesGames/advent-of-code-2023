#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(Vector2dTests, ConstructionInitializesToZero)
{
	auto vector = Utilities::Vector2d<int>{};
	ASSERT_EQ(vector.x, 0);
	ASSERT_EQ(vector.y, 0);
}

TEST(Vector2dTests, AdditionOperator)
{
	auto left = Utilities::Vector2d<int>{ 1, 2 };
	auto right = Utilities::Vector2d<int>{ 3, 4 };
	auto sum = left + right;
	ASSERT_EQ(sum.x, 4);
	ASSERT_EQ(sum.y, 6);
}

TEST(Vector2dTests, SubtractionOperator)
{
	auto left = Utilities::Vector2d<int>{ 1, 2 };
	auto right = Utilities::Vector2d<int>{ 3, 4 };
	auto difference = left - right;
	ASSERT_EQ(difference.x, -2);
	ASSERT_EQ(difference.y, -2);
}

TEST(Vector2dTests, MultiplicationOperator)
{
	auto vec = Utilities::Vector3d<int>{ 1, 2 } * 5;
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 10);
}

TEST(Vector2dTests, AdditionAssignmentOperator)
{
	auto vec = Utilities::Vector2d<int>{ 1, 2 };
	vec += Utilities::Vector2d<int>{ 3, 4 };
	ASSERT_EQ(vec.x, 4);
	ASSERT_EQ(vec.y, 6);
}

TEST(Vector2dTests, SubtractionAssignmentOperator)
{
	auto vec = Utilities::Vector2d<int>{ 1, 2 };
	vec -= Utilities::Vector2d<int>{ 3, 4 };
	ASSERT_EQ(vec.x, -2);
	ASSERT_EQ(vec.y, -2);
}

TEST(Vector2dTests, MultiplicationAssignmentOperator)
{
	auto vec = Utilities::Vector2d<int>{ 1, 2 };
	vec *= 5;
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 10);
}

TEST(Vector2dTests, ComparisonOperators)
{
	{
		auto left = Utilities::Vector2d<int>{ 2, 3 };
		auto right = Utilities::Vector2d<int>{ 2, 3 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::Vector2d<int>{ 1, 2 };
		auto right = Utilities::Vector2d<int>{ 2, 3 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::Vector2d<int>{ 2, 3 };
		auto right = Utilities::Vector2d<int>{ 1, 4 };
		EXPECT_LT(right, left);
		EXPECT_NE(left, right);
		EXPECT_GT(left, right);
	}
}

TEST(Vector2dTests, Hashing)
{
	{
		auto hash = std::hash<Utilities::Vector2d<int>>();
		auto left = hash(Utilities::Vector2d<int>{ 2, 3 });
		auto right = hash(Utilities::Vector2d<int>{ 2, 3 });
		EXPECT_EQ(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector2d<int>>();
		auto left = hash(Utilities::Vector2d<int>{ 2, 3 });
		auto right = hash(Utilities::Vector2d<int>{ 2, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector2d<int>>();
		auto left = hash(Utilities::Vector2d<int>{ 2, 3 });
		auto right = hash(Utilities::Vector2d<int>{ 5, 3 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector2d<int64_t>>();
		auto left = hash(Utilities::Vector2d<int64_t>{ 2, 3 });
		auto right = hash(Utilities::Vector2d<int64_t>{ 2, 3 });
		EXPECT_EQ(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector2d<int64_t>>();
		auto left = hash(Utilities::Vector2d<int64_t>{ 2, 3 });
		auto right = hash(Utilities::Vector2d<int64_t>{ 2, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector2d<int64_t>>();
		auto left = hash(Utilities::Vector2d<int64_t>{ 2, 3 });
		auto right = hash(Utilities::Vector2d<int64_t>{ 5, 3 });
		EXPECT_NE(left, right);
	}
}

TEST(Vector2dTests, ToString)
{
	auto vec = Utilities::Vector2d<int>{ 1, 2};
	EXPECT_EQ(vec.ToString(), "(1, 2)");
}