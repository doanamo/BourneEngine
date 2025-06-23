#include "Shared.hpp"

TEST_DEFINE("Common.ScopeValue", "ExitScope")
{
    bool value = false;
    {
        ScopeValue scope(value, true);
        TEST_TRUE(value);
    }
    TEST_FALSE(value);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}
