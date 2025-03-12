#include "Shared.hpp"
#include "TestCommon.hpp"

Test::Result Common::TestScopeValue()
{
    LOG_INFO("Running Common::TestScopeValue...");
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
