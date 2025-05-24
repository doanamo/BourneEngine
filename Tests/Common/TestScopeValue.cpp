#include "Shared.hpp"

TEST_DEFINE("Common.ScopeValue")
{
    const Test::MemoryGuard memoryStats;

    // Test scope value
    {
        bool value = false;
        {
            ScopeValue scope(value, true);
            TEST_TRUE(value);
        }
        TEST_FALSE(value);
    }

    TEST_TRUE(memoryStats.ValidateCurrentAllocations(0, 0));
    return Test::Result::Success;
}
