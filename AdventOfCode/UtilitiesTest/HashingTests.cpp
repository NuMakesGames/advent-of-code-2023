#include "pch.h"
#include "../Utilities/Utilities.h"

TEST(HashCombineTests, SingleValueOverload)
{
	{
		auto seed = 0ull;
		Utilities::HashCombine(seed, 42);
		EXPECT_NE(seed, 0);
	}

	{
		auto seed = 0ull;
		Utilities::HashCombine(seed, 42);
		auto afterFirst = seed;
		Utilities::HashCombine(seed, 42);
		auto afterSecond = seed;
		EXPECT_NE(afterFirst, afterSecond);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		Utilities::HashCombine(firstSeed, 42);
		Utilities::HashCombine(secondSeed, 42);
		EXPECT_EQ(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		Utilities::HashCombine(firstSeed, 42);
		Utilities::HashCombine(secondSeed, 43);
		EXPECT_NE(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 9ull;
		auto secondSeed = 17ull;
		Utilities::HashCombine(firstSeed, 42);
		Utilities::HashCombine(secondSeed, 42);
		EXPECT_NE(firstSeed, secondSeed);
	}
}

TEST(HashCombineTests, VariadicOverload)
{
	{
		auto seed = 0ull;
		Utilities::HashCombine(seed, 42, 17, 13);
		EXPECT_NE(seed, 0);
	}

	{
		auto seed = 0ull;
		Utilities::HashCombine(seed, 42, 17, 13);
		auto afterFirst = seed;
		Utilities::HashCombine(seed, 42, 17, 13);
		auto afterSecond = seed;
		EXPECT_NE(afterFirst, afterSecond);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		Utilities::HashCombine(firstSeed, 42, 17, 13);
		Utilities::HashCombine(secondSeed, 42, 17, 13);
		EXPECT_EQ(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		Utilities::HashCombine(firstSeed, 42, 17, 13);
		Utilities::HashCombine(secondSeed, 42, 18, 13);
		EXPECT_NE(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 9ull;
		auto secondSeed = 17ull;
		Utilities::HashCombine(firstSeed, 42, 17, 13);
		Utilities::HashCombine(secondSeed, 42, 17, 13);
		EXPECT_NE(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		Utilities::HashCombine(firstSeed, 1, 2, 3);
		Utilities::HashCombine(secondSeed, 2, 1, 3);
		EXPECT_NE(firstSeed, secondSeed);
	}
}

TEST(HashRangeTests, DoesNotChangeSeedOnEmptyRange)
{
	auto seed = 0ull;
	auto range = std::vector<int>{};
	Utilities::HashRange(seed, range.begin(), range.end());
	EXPECT_EQ(seed, 0);
}

TEST(HashRangeTests, HandlesNonEmptyRanges)
{
	{
		auto seed = 0ull;
		auto range = std::vector<int>{ 1, 2, 3 };
		Utilities::HashRange(seed, range.begin(), range.end());
		EXPECT_NE(seed, 0);
	}

	{
		auto seed = 0ull;
		auto range = std::vector<int>{ 1, 2, 3 };
		Utilities::HashRange(seed, range.begin(), range.end());
		auto afterFirst = seed;
		Utilities::HashRange(seed, range.begin(), range.end());
		auto afterSecond = seed;
		EXPECT_NE(afterFirst, afterSecond);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		auto range = std::vector<int>{ 1, 2, 3 };
		Utilities::HashRange(firstSeed, range.begin(), range.end());
		Utilities::HashRange(secondSeed, range.begin(), range.end());
		EXPECT_EQ(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 0ull;
		auto secondSeed = 0ull;
		auto range = std::vector<int>{ 1, 2, 3 };
		Utilities::HashRange(firstSeed, range.begin(), range.end());
		Utilities::HashRange(secondSeed, range.rbegin(), range.rend());
		EXPECT_NE(firstSeed, secondSeed);
	}

	{
		auto firstSeed = 9ull;
		auto secondSeed = 17ull;
		auto range = std::vector<int>{ 1, 2, 3 };
		Utilities::HashRange(firstSeed, range.begin(), range.end());
		Utilities::HashRange(secondSeed, range.begin(), range.end());
		EXPECT_NE(firstSeed, secondSeed);
	}
}
