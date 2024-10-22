#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestUtility()
{
    LOG_INFO("Running Common::TestUtility...");

    // Test ArraySize() function
    char staticArray[63];
    TEST_TRUE(ArraySize(staticArray) == 63);

    // Test Min() and Max() functions
    TEST_TRUE(Min(1, 2) == 1);
    TEST_TRUE(Min(2, 1) == 1);
    TEST_TRUE(Max(1, 2) == 2);
    TEST_TRUE(Max(2, 1) == 2);

    // Test IsPow2() function
    TEST_TRUE(IsPow2(1u));
    TEST_TRUE(IsPow2(2u));
    TEST_FALSE(IsPow2(3u));
    TEST_TRUE(IsPow2(4u));
    TEST_FALSE(IsPow2(5u));
    TEST_FALSE(IsPow2(6u));
    TEST_FALSE(IsPow2(7u));
    TEST_TRUE(IsPow2(8u));

    TEST_TRUE(IsPow2(1ull));
    TEST_TRUE(IsPow2(2ull));
    TEST_FALSE(IsPow2(3ull));
    TEST_TRUE(IsPow2(4ull));
    TEST_FALSE(IsPow2(5ull));
    TEST_FALSE(IsPow2(6ull));
    TEST_FALSE(IsPow2(7ull));
    TEST_TRUE(IsPow2(8ull));

    // Test NextPow2() function
    TEST_TRUE(NextPow2(0u) == 1u);
    TEST_TRUE(NextPow2(1u) == 2u);
    TEST_TRUE(NextPow2(2u) == 4u);
    TEST_TRUE(NextPow2(3u) == 4u);
    TEST_TRUE(NextPow2(4u) == 8u);
    TEST_TRUE(NextPow2(5u) == 8u);
    TEST_TRUE(NextPow2(6u) == 8u);
    TEST_TRUE(NextPow2(7u) == 8u);
    TEST_TRUE(NextPow2(8u) == 16u);

    TEST_TRUE(NextPow2(0ull) == 1ull);
    TEST_TRUE(NextPow2(1ull) == 2ull);
    TEST_TRUE(NextPow2(2ull) == 4ull);
    TEST_TRUE(NextPow2(3ull) == 4ull);
    TEST_TRUE(NextPow2(4ull) == 8ull);
    TEST_TRUE(NextPow2(5ull) == 8ull);
    TEST_TRUE(NextPow2(6ull) == 8ull);
    TEST_TRUE(NextPow2(7ull) == 8ull);
    TEST_TRUE(NextPow2(8ull) == 16ull);

    TEST_TRUE(NextPow2(1u << (31 - 1)) == (1u << 31));
    TEST_TRUE(NextPow2(1ull << (63 - 1)) == (1ull << 63));

    return TestResult::Success;
}
