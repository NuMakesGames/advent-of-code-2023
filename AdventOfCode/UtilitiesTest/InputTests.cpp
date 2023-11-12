#include "pch.h"
#include "../Utilities/Utilities.h"

TEST(ReadAllLinesInFileTests, HandlesFileThatDoesNotExist)
{
	auto result = Utilities::ReadAllLinesInFile("BogusFile.txt");
	EXPECT_EQ(result.size(), 0);
}

TEST(ReadAllLinesInFileTests, HandlesBasicTextFile)
{
	auto result = Utilities::ReadAllLinesInFile("ReadAllLinesInFileTests.input");
	ASSERT_EQ(result.size(), 8);
	EXPECT_EQ(result[0], "abcd efgh ijkl");
	EXPECT_EQ(result[1], "the cat in the hat");
	EXPECT_EQ(result[2], "1");
	EXPECT_EQ(result[3], "\t");
	EXPECT_EQ(result[4], "2");
	EXPECT_EQ(result[5], "3");
	EXPECT_EQ(result[6], "");
	EXPECT_EQ(result[7], "15 18 19");
}

TEST(ReadAllLinesInFileAsGridTests, HandlesBasicTextFile)
{
	auto lines = Utilities::ReadAllLinesInFile("ReadAllLinesInFileAsGridTests.input");
	auto grid = Utilities::ReadAllLinesInFileAsGrid("ReadAllLinesInFileAsGridTests.input");

	ASSERT_EQ(grid.Width(), 21);
	ASSERT_EQ(grid.Height(), 19);

	for (int y = 0; y < grid.Height(); ++y)
	{
		for (int x = 0; x < grid.Width(); ++x)
		{
			EXPECT_EQ(grid.at(x, y), lines[y][x]);
		}
	}
}

TEST(SplitStringTests, HandlesEmptyInput)
{
	auto result = Utilities::SplitString("", ",");
	EXPECT_EQ(result.size(), 0);
}

TEST(SplitStringTests, HandlesEmptyDelimiter)
{
	auto result = Utilities::SplitString("test", "");
	ASSERT_EQ(result.size(), 4);
	EXPECT_EQ(result[0], "t");
	EXPECT_EQ(result[1], "e");
	EXPECT_EQ(result[2], "s");
	EXPECT_EQ(result[3], "t");
}

TEST(SplitStringTests, HandlesSimpleDelimiters)
{
	{
		auto result = Utilities::SplitString("this is a test", " ");
		ASSERT_EQ(result.size(), 4);
		EXPECT_EQ(result[0], "this");
		EXPECT_EQ(result[1], "is");
		EXPECT_EQ(result[2], "a");
		EXPECT_EQ(result[3], "test");
	}

	{
		auto result = Utilities::SplitString("1,2,3,4,5,6,7,8,9,10", ",");
		ASSERT_EQ(result.size(), 10);
		for (auto i = 0; i < result.size(); ++i)
		{
			EXPECT_EQ(result[i], std::to_string(i + 1));
		}
	}
}

TEST(SplitStringTests, HandlesTrailingDelimiter)
{
	auto result = Utilities::SplitString("a,b,c,", ",");
	ASSERT_EQ(result.size(), 4);
	EXPECT_EQ(result[0], "a");
	EXPECT_EQ(result[1], "b");
	EXPECT_EQ(result[2], "c");
	EXPECT_EQ(result[3], "");
}

TEST(SplitStringTests, HandlesEmptyTokens)
{
	auto result = Utilities::SplitString("a,b,,c", ",");
	ASSERT_EQ(result.size(), 4);
	EXPECT_EQ(result[0], "a");
	EXPECT_EQ(result[1], "b");
	EXPECT_EQ(result[2], "");
	EXPECT_EQ(result[3], "c");
}

TEST(SplitStringTests, HandlesComplexDelimiters)
{
	{
		auto result = Utilities::SplitString("oh,!my", ",!");
		ASSERT_EQ(result.size(), 2);
		EXPECT_EQ(result[0], "oh");
		EXPECT_EQ(result[1], "my");
	}

	{
		auto result = Utilities::SplitString("the catcat in the hat laughed at a cat", "cat");
		ASSERT_EQ(result.size(), 4);
		EXPECT_EQ(result[0], "the ");
		EXPECT_EQ(result[1], "");
		EXPECT_EQ(result[2], " in the hat laughed at a ");
		EXPECT_EQ(result[3], "");
	}
}

TEST(SplitCommaSeparatedInt32sTests, HandlesEmptyInput)
{
	auto result = Utilities::SplitCommaSeparatedInt32s("");
	ASSERT_EQ(result.size(), 0);
}

TEST(SplitCommaSeparatedInt32sTests, HandlesSimpleCase)
{
	auto result = Utilities::SplitCommaSeparatedInt32s("-1,0,1,2,3");
	ASSERT_EQ(result.size(), 5);
	for (auto i = 0; i < result.size(); ++i)
	{
		EXPECT_EQ(result[i], i - 1);
	}
}

TEST(SplitCommaSeparatedInt32sTests, HandlesInt32MinAndInt32Max)
{
	auto result = Utilities::SplitCommaSeparatedInt32s("-2147483648,2147483647");
	ASSERT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], INT32_MIN);
	EXPECT_EQ(result[1], INT32_MAX);
}

TEST(SplitCommaSeparatedInt64sTests, HandlesEmptyInput)
{
	auto result = Utilities::SplitCommaSeparatedInt64s("");
	ASSERT_EQ(result.size(), 0);
}

TEST(SplitCommaSeparatedInt64sTests, HandlesSimpleCase)
{
	auto result = Utilities::SplitCommaSeparatedInt64s("-1,0,1,2,3");
	ASSERT_EQ(result.size(), 5);
	for (auto i = 0; i < result.size(); ++i)
	{
		EXPECT_EQ(result[i], i - 1);
	}
}

TEST(SplitCommaSeparatedInt64sTests, HandlesInt64MinAndInt64Max)
{
	auto result = Utilities::SplitCommaSeparatedInt64s("-9223372036854775808,9223372036854775807");
	ASSERT_EQ(result.size(), 2);
	EXPECT_EQ(result[0], INT64_MIN);
	EXPECT_EQ(result[1], INT64_MAX);
}