#include "Shared.hpp"

TEST_DEFINE("Common.ScopeGuard")
{
    // Test scope guard macro
    {
        Test::MemoryGuard memoryGuard;

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

        TEST_TRUE(memoryGuard.ValidateCurrentAllocations(0, 0));
    }

    return Test::Result::Success;
}
