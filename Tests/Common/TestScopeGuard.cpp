#include "Shared.hpp"

TEST_DEFINE("Common.ScopeGuard")
{
    const Test::MemoryStats memoryStats;

    // Test scope guard macro
    {
        bool called = false;
        {
            SCOPE_GUARD
            {
                called = true;
            };

            SCOPE_GUARD
            {
                // Checks if multiple guards can be declared in same scope
            };
        }
        TEST_TRUE(called);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}
