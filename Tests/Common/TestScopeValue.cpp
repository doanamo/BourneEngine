#include "Shared.hpp"

TEST_DEFINE("Common.ScopeValue")
{
    const Test::MemoryStats memoryStats;

    // Test scope value
    {
        bool value = false;
        {
            ScopeValue scope(value, true);
            TEST_TRUE(value);
        }
        TEST_FALSE(value);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}
