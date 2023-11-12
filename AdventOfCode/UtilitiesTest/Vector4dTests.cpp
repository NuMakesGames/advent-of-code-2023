#include "pch.h"

#include "../Utilities/Utilities.h"

TEST(Vector4dTests, ConstructionInitializesToZero)
{
	auto vector = Utilities::Vector4d<int>{};
	ASSERT_EQ(vector.x, 0);
	ASSERT_EQ(vector.y, 0);
	ASSERT_EQ(vector.z, 0);
}

TEST(Vector4dTests, AdditionOperator)
{
	auto left = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
	auto right = Utilities::Vector4d<int>{ 4, 5, 6, 7 };
	auto sum = left + right;
	ASSERT_EQ(sum.x, 5);
	ASSERT_EQ(sum.y, 7);
	ASSERT_EQ(sum.z, 9);
	ASSERT_EQ(sum.w, 11);
}

TEST(Vector4dTests, SubtractionOperator)
{
	auto left = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
	auto right = Utilities::Vector4d<int>{ 4, 5, 6, 7 };
	auto difference = left - right;
	ASSERT_EQ(difference.x, -3);
	ASSERT_EQ(difference.y, -3);
	ASSERT_EQ(difference.z, -3);
	ASSERT_EQ(difference.w, -3);
}

TEST(Vector4dTests, MultiplicationOperator)
{
	auto vec = Utilities::Vector4d<int>{ 1, 2, 3, 4 } * 5;
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 10);
	ASSERT_EQ(vec.z, 15);
	ASSERT_EQ(vec.w, 20);
}

TEST(Vector4dTests, AdditionAssignmentOperator)
{
	auto vec = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
	vec += Utilities::Vector4d<int>{ 4, 5, 6, 7 };
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 7);
	ASSERT_EQ(vec.z, 9);
	ASSERT_EQ(vec.w, 11);
}

TEST(Vector4dTests, SubtractionAssignmentOperator)
{
	auto vec = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
	vec -= Utilities::Vector4d<int>{ 4, 5, 6, 7 };
	ASSERT_EQ(vec.x, -3);
	ASSERT_EQ(vec.y, -3);
	ASSERT_EQ(vec.z, -3);
	ASSERT_EQ(vec.w, -3);
}

TEST(Vector4dTests, MultiplicationAssignmentOperator)
{
	auto vec = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
	vec *= 5;
	ASSERT_EQ(vec.x, 5);
	ASSERT_EQ(vec.y, 10);
	ASSERT_EQ(vec.z, 15);
	ASSERT_EQ(vec.w, 20);
}

TEST(Vector4dTests, ComparisonOperators)
{
	{
		auto left = Utilities::Vector4d<int>{ 2, 3, 4, 5 };
		auto right = Utilities::Vector4d<int>{ 2, 3, 4, 5 };
		EXPECT_EQ(left, right);
	}

	{
		auto left = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
		auto right = Utilities::Vector4d<int>{ 2, 3, 4, 5 };
		EXPECT_LT(left, right);
		EXPECT_NE(left, right);
		EXPECT_GT(right, left);
	}

	{
		auto left = Utilities::Vector4d<int>{ 2, 3, 4, 5 };
		auto right = Utilities::Vector4d<int>{ 1, 4, 5, 6 };
		EXPECT_LT(right, left);
		EXPECT_NE(left, right);
		EXPECT_GT(left, right);
	}
}

TEST(Vector4dTests, Hashing)
{
	{
		auto hash = std::hash<Utilities::Vector4d<int>>();
		auto left = hash(Utilities::Vector4d<int>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int>{ 2, 3, 4, 5 });
		EXPECT_EQ(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int>>();
		auto left = hash(Utilities::Vector4d<int>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int>{ 2, 3, 4, 6 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int>>();
		auto left = hash(Utilities::Vector4d<int>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int>{ 2, 3, 6, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int>>();
		auto left = hash(Utilities::Vector4d<int>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int>{ 2, 6, 4, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int>>();
		auto left = hash(Utilities::Vector4d<int>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int>{ 6, 3, 4, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int64_t>>();
		auto left = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 5 });
		EXPECT_EQ(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int64_t>>();
		auto left = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 6 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int64_t>>();
		auto left = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int64_t>{ 2, 3, 6, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int64_t>>();
		auto left = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int64_t>{ 2, 6, 4, 5 });
		EXPECT_NE(left, right);
	}

	{
		auto hash = std::hash<Utilities::Vector4d<int64_t>>();
		auto left = hash(Utilities::Vector4d<int64_t>{ 2, 3, 4, 5 });
		auto right = hash(Utilities::Vector4d<int64_t>{ 6, 3, 4, 5 });
		EXPECT_NE(left, right);
	}
}

TEST(Vector4dTests, ToString)
{
	auto vec = Utilities::Vector4d<int>{ 1, 2, 3, 4 };
	EXPECT_EQ(vec.ToString(), "(1, 2, 3, 4)");
}