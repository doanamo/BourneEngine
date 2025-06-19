#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

Test::Result RunTest(const Test::Entry& testEntry)
{
    LOG_INFO("Running test: %.*s.%.*s", STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));

    Test::MemoryGuard memoryStats;
    Test::ObjectGuard objectGuard;

    Test::Result result = testEntry.function();
    if(result != Test::Result::Success)
    {
        LOG_ERROR("Test \" %.*s.%.*s\" failed!", STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));
    }

    return result;
}

Test::Result RunAllTests()
{
    // #todo: Print how many groups will be executed.
    LOG_INFO("Running all %lu tests...", Test::Registry::GetTests().GetSize());

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
    LOG_INFO("Printing %lu available tests:", Test::Registry::GetTests().GetSize());
    LOG_NO_SOURCE_LINE_SCOPE();

    for(const Test::Entry& testEntry : Test::Registry::GetTests())
    {
        LOG_INFO("  %.*s.%.*s", STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));
    }
}

bool WriteTests(const String& outputPath)
{
    LOG_INFO("Writing %lu discovered test groups...", Test::Registry::GetGroups().GetSize());

    HeapString builder;
    for(const StringView& testGroup : Test::Registry::GetGroups())
    {
        builder.Append("add_test(NAME %.*s COMMAND Tests -RunTest=\"%.*s.\")\n",
            STRING_VIEW_PRINTF_ARG(testGroup), STRING_VIEW_PRINTF_ARG(testGroup));
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

    // #todo: Sort discovered test groups (not names which should remain in order of definition).

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
    else if(const auto& testQuery = commandLine.GetArgumentValue("RunTest"))
    {
        u32 testsFound = 0;
        u32 testsFailed = 0;
        for(const Test::Entry& testEntry : Test::Registry::GetTests())
        {
            // #todo: Find all tests first so we can print how many there will be executed.
            auto fullTestName = InlineString<128>::Format("%.*s.%.*s",
                STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));

            // #todo: How to avoid conversion to string view?
            if(StringView(fullTestName).StartsWith(testQuery.GetValue()))
            {
                ++testsFound;
                if(RunTest(testEntry) != Test::Result::Success)
                {
                    ++testsFailed;
                }
            }
        }

        if(testsFound == 0)
        {
            LOG_ERROR("Failed to find any tests for \"%.*s\" query", STRING_VIEW_PRINTF_ARG(testQuery.GetValue()));
            return -1;
        }

        if(testsFailed > 0)
        {
            LOG_ERROR("Test execution was unsuccessful due to %u failures", testsFailed);
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
