#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

Test::Result RunTest(const Test::Entry& testEntry)
{
    LOG_INFO("Running \"" STRING_VIEW_PRINTF "\" test...", STRING_VIEW_VARG(testEntry.name));

    Test::MemoryStats memoryStats;
    Test::Object::ResetGlobalCounters();
    Test::Result result = testEntry.function();
    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return result;
}

Test::Result RunAllTests()
{
    LOG_INFO("Running all tests...");

    bool testsSucceeded = true;
    for(const Test::Entry& testEntry : Test::Registry::Get().GetTests())
    {
        if (RunTest(testEntry) != Test::Result::Success)
        {
            testsSucceeded = false;
        }
    }

    return testsSucceeded ? Test::Result::Success : Test::Result::Failure;
}

void ListTests()
{
}

int main(const int argc, const char* const* argv)
{
    Engine::Setup({
        .applicationName = "Bourne Engine Tests",
        .commandLineArguments = argv,
        .commandLineArgumentCount = argc,
    });

    if(Platform::CommandLine::Get().HasArgument("ListTests"))
    {
        ListTests();
    }
    else
    {
        if(RunAllTests() != Test::Result::Success)
        {
            LOG_ERROR("Test execution has failed");
            return -1;
        }

        LOG_SUCCESS("Test execution was successful");
    }

    return 0;
}
