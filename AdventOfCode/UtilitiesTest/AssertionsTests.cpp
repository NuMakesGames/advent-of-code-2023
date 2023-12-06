#include "pch.h"
#include "../Utilities/Utilities.h"

TEST(AssertionsDeathTest, VerifyElseCrashFailureKillsProcess)
{
	ASSERT_DEATH(VerifyElseCrash(false), "");
}

TEST(AssertionsDeathTest, VerifyElseCrashSuccessDoesNotKillProcess)
{
	VerifyElseCrash(true);
}

TEST(AssertionsDeathTest, VerifyNotReachedKillsProcess)
{
	ASSERT_DEATH(Utilities::VerifyNotReached(), "");
}