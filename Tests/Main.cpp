#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

Test::Result RunTest(const Test::Entry& testEntry)
{
    LOG_INFO("Running \"%.*s\" test...", STRING_VIEW_PRINTF_ARG(testEntry.name));

    Test::MemoryGuard memoryStats;
    Test::ObjectGuard objectGuard;
    return testEntry.function();
}

Test::Result RunAllTests()
{
    LOG_INFO("Running all %lu tests...", Test::Registry::GetTestCount());

    bool testsSucceeded = true;
    for(const Test::Entry& testEntry : Test::Registry::GetTests())
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
    LOG_INFO("Printing %lu available tests:", Test::Registry::GetTestCount());
    LOG_NO_SOURCE_LINE_SCOPE();

    for(const Test::Entry& testEntry : Test::Registry::GetTests())
    {
        LOG_INFO("  %.*s", STRING_VIEW_PRINTF_ARG(testEntry.name));
    }
}

bool WriteTests(const String& outputPath)
{
    LOG_INFO("Writing %lu discovered tests...", Test::Registry::GetTestCount());

    HeapString builder;
    for(const Test::Entry& testEntry : Test::Registry::GetTests())
    {
        builder.Append("add_test(NAME %.*s COMMAND Tests -RunTest=\"%.*s\")\n",
            STRING_VIEW_PRINTF_ARG(testEntry.name), STRING_VIEW_PRINTF_ARG(testEntry.name));
    }

    return WriteStringToFileIfDifferent(outputPath, builder);
}

int main(const int argc, const char* const* argv)
{
    {
        LOG_TOGGLE_ENABLED_SCOPE(false);

        Engine::Setup({
            .applicationName = "Bourne Engine Tests",
            .commandLineArguments = argv,
            .commandLineArgumentCount = argc
        });
    }

    const auto& commandLine = Platform::CommandLine::Get();
    if(commandLine.HasArgument("ListTests"))
    {
        ListTests();
    }
    else if(const auto& outputPath = commandLine.GetArgumentValue("WriteTests"))
    {
        if(!WriteTests(*outputPath))
        {
            LOG_ERROR("Failed to write discovered tests");
            return -1;
        }

        LOG_SUCCESS("Discovered test written to: %.*s", STRING_VIEW_PRINTF_ARG(outputPath.GetValue()));
    }
    else if(const auto& testName = commandLine.GetArgumentValue("RunTest"))
    {
        const Test::Entry* foundTestEntry = Test::Registry::GetTests()
            .FindPredicate([&testName](const Test::Entry& testEntry)
            {
                return testEntry.name == *testName;
            });

        if(foundTestEntry == nullptr)
        {
            LOG_ERROR("Failed to run non-existing \"%.*s\" test", STRING_VIEW_PRINTF_ARG(testName.GetValue()));
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
