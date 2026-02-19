#include <gtest/gtest.h>

// All of the below is to be replaced later, these are just placeholders for testing

// Test if linking works
#include "FPSLimiter.h"

// Simple smoke test
TEST(SmokeTest, BasicMathTest)
{
    EXPECT_EQ(2 + 2, 4);
}

TEST(SmokeTest, CanConstructFPSLimiterClass)
{
    // This just proves headers compile and symbols link
    FPSLimiter limiter;

    SUCCEED();
}