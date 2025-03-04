#include "Shared.hpp"
#include "TestCommon.hpp"

Test::Result Common::TestScopeGuard()
{
    LOG_INFO("Running Common::TestScopeGuard...");
    const Test::MemoryStats memoryStats;

    // Test SCOPE_GUARD macro
    {
        bool called = false;
        {
            SCOPE_GUARD
            {
                called = true;
            };
        }
        TEST_TRUE(called);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}
