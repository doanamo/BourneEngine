#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

Test::Result RunTest(const Test::Entry& testEntry)
{
    LOG_INFO("Running \"" STRING_VIEW_FORMAT "\" test...", STRING_VIEW_VARG(testEntry.name));

    Test::MemoryStats memoryStats;
    Test::Object::ResetGlobalCounters();
    Test::Result result = testEntry.function();
    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return result;
}

Test::Result RunAllTests()
{
    LOG_INFO("Running all tests");

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
    LOG_INFO("Printing available tests:");
    LOG_NO_SOURCE_LINE_SCOPE();

    for(const Test::Entry& testEntry : Test::Registry::Get().GetTests())
    {
        LOG_INFO("  " STRING_VIEW_FORMAT, STRING_VIEW_VARG(testEntry.name));
    }
}

void WriteTests()
{

}

int main(const int argc, const char* const* argv)
{
    Engine::Setup({
        .applicationName = "Bourne Engine Tests",
        .commandLineArguments = argv,
        .commandLineArgumentCount = argc,
    });

    const auto& commandLine = Platform::CommandLine::Get();
    if(commandLine.HasArgument("ListTests"))
    {
        ListTests();
    }
    else if(commandLine.HasArgument("WriteTests"))
    {
        WriteTests();
    }
    else if(const auto& testName = commandLine.GetArgumentValue("RunTest"))
    {
        const Test::Entry* foundTestEntry = Test::Registry::Get().GetTests()
            .FindPredicate([&testName](const Test::Entry& testEntry)
            {
                return testEntry.name == *testName;
            });

        if(foundTestEntry == nullptr)
        {
            LOG_ERROR("Failed to run non-existing \"" STRING_VIEW_FORMAT "\" test", STRING_VIEW_VARG(testName.GetValue()));
            return -1;
        }

        if(RunTest(*foundTestEntry) != Test::Result::Success)
        {
            LOG_ERROR("Test execution has failed");
            return -1;
        }

        LOG_SUCCESS("Test execution was successful");
    }
    else
    {
        if(RunAllTests() != Test::Result::Success)
        {
            LOG_ERROR("All test execution has failed");
            return -1;
        }

        LOG_SUCCESS("All test execution was successful");
    }

    return 0;
}
