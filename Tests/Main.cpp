#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

void ListTests()
{
    LOG_INFO("Printing %lu available test(s) from %lu group(s):",
        Test::Registry::GetTests().GetSize(),
        Test::Registry::GetGroups().GetSize());

    LOG_NO_SOURCE_LINE_SCOPE();
    for(const Test::Entry& testEntry : Test::Registry::GetTests())
    {
        LOG_INFO("  %.*s.%.*s", STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));
    }
}

int WriteTests(const String& outputPath)
{
    LOG_INFO("Writing %lu discovered test group(s)...", Test::Registry::GetGroups().GetSize());

    HeapString builder;
    for(const StringView& testGroup : Test::Registry::GetGroups())
    {
        builder.Append("add_test(%.*s Tests -RunTests=\"%.*s.\")\n",
            STRING_VIEW_PRINTF_ARG(testGroup), STRING_VIEW_PRINTF_ARG(testGroup));
    }

    if(!WriteStringToFileIfDifferent(outputPath, builder))
    {
        LOG_ERROR("Failed to write discovered tests");
        return -1;
    }

    LOG_SUCCESS("Discovered tests written to: %s", *outputPath);
    return 0;
}

int RunTests(const StringView& testQuery)
{
    Array<const Test::Entry*> foundTests;
    for(const Test::Entry& testEntry : Test::Registry::GetTests())
    {
        auto fullTestName = InlineString<128>::Format("%.*s.%.*s",
            STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));

        if(fullTestName.StartsWith(testQuery))
        {
            foundTests.Add(&testEntry);
        }
    }

    if(foundTests.IsEmpty())
    {
        LOG_ERROR("Failed to find any tests for \"%.*s\" query", STRING_VIEW_PRINTF_ARG(testQuery));
        return -1;
    }

    LOG_INFO("Found %lu test(s) that match \"%.*s\" query", foundTests.GetSize(), STRING_VIEW_PRINTF_ARG(testQuery));

    u32 testsFailed = 0;
    for(auto& foundTest : foundTests)
    {
        if(foundTest->Run() != Test::Result::Success)
        {
            ++testsFailed;
        }
    }

    if(testsFailed > 0)
    {
        LOG_ERROR("Test execution was unsuccessful due to %u failures", testsFailed);
        return -1;
    }

    LOG_SUCCESS("Test execution was successful");
    return 0;
}

int RunAllTests()
{
    LOG_INFO("Running all %lu test(s) from %lu group(s)...",
        Test::Registry::GetTests().GetSize(),
        Test::Registry::GetGroups().GetSize());

    bool testsSucceeded = true;
    for(const Test::Entry& testEntry : Test::Registry::GetTests())
    {
        // #todo: Test assertion macros trigger DEBUG_BREAK(), which in turn calls SIGINT, which aborts program.
        // We want DEBUG_BREAK() to only break when debugger is attached. We may have to implement some platform
        // specific break functions.
        // See: https://stackoverflow.com/questions/173618/is-there-a-portable-equivalent-to-debugbreak-debugbreak
        if (testEntry.Run() != Test::Result::Success)
        {
            testsSucceeded = false;
        }
    }

    if(!testsSucceeded)
    {
        LOG_ERROR("All test execution has failed");
        return -1;
    }

    LOG_SUCCESS("All test execution was successful");
    return 0;
}

int main(const int argc, const char* const* argv)
{
    Engine::Config config;
    config.applicationName = "Bourne Engine Tests";
    config.commandLineArgv = argv;
    config.commandLineArgc = argc;
    config.headless = true;

    Engine::Engine engine;
    {
        LOG_MINIMUM_SEVERITY_SCOPE(Logger::Severity::Warning);
        if(!engine.Setup(config))
        {
            LOG_FATAL("Failed to setup engine");
            return -1;
        }
    }

    // #todo: Sort discovered test groups (not names which should remain in order of definition).

    const auto& commandLine = Platform::CommandLine::Get();
    if(commandLine.HasArgument("ListTests"))
    {
        ListTests();
    }
    else if(const auto& outputPath = commandLine.GetArgumentValue("WriteTests"))
    {
        return WriteTests(*outputPath);
    }
    else if(const auto& testQuery = commandLine.GetArgumentValue("RunTests"))
    {
        return RunTests(*testQuery);
    }
    else
    {
        return RunAllTests();
    }

    return 0;
}
