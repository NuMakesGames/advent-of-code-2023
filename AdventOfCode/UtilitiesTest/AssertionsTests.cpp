#include "pch.h"
#include "../Utilities/Utilities.h"

TEST(AssertionsDeathTest, VerifyElseCrashFailureKillsProcess)
{
	ASSERT_DEATH(Utilities::VerifyElseCrash(false), "");
}

TEST(AssertionsDeathTest, VerifyElseCrashSuccessDoesNotKillProcess)
{
	Utilities::VerifyElseCrash(true);
}

TEST(AssertionsDeathTest, VerifyNotReachedKillsProcess)
{
	ASSERT_DEATH(Utilities::VerifyNotReached(), "");
}