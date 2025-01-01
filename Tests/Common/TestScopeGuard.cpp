#include "Shared.hpp"
#include "Memory/TestMemoryStats.hpp"

Test::Result Common::TestScopeGuard()
{
    LOG_INFO("Running Common::TestResult...");
    const Memory::TestStats memoryStats;

    // Test basic functionality
    {
        bool called = false;
        {
            auto guard = MakeScopeGuard([&]() { called = true; });
        }
        TEST_TRUE(called);
    }

    // Test move constructor
    {
        bool called = false;
        {
            auto guard1 = MakeScopeGuard([&]() { called = true; });
            auto guard2 = std::move(guard1);
        }
        TEST_TRUE(called);
    }

    // Test SCOPE_GUARD macro
    {
        bool called = false;
        {
            SCOPE_GUARD([&]() { called = true; });
        }
        TEST_TRUE(called);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}
