#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

class TestsApplication final : public Application
{
public:
    Config GetConfig() override;

    bool OnSetup() override;
    Optional<ExitCode> OnRun() override;

private:
    void ListTests();
    ExitCode DiscoverTests(const String& outputPath);
    ExitCode RunTests(const StringView& testQuery);
    ExitCode RunAllTests();
};

DEFINE_PRIMARY_APPLICATION("Bourne Engine Tests", TestsApplication);

Config TestsApplication::GetConfig()
{
    Config config;
    config.logger.minimumSeverity = Logger::Severity::Warning;
    config.headless = true;
    return config;
}

bool TestsApplication::OnSetup()
{
    if(!Test::Registry::Setup())
    {
        LOG_ERROR("Failed to setup test registry");
        return false;
    }

    return true;
}

Optional<ExitCode> TestsApplication::OnRun()
{
    LOG_MINIMUM_SEVERITY_SCOPE(Logger::Severity::Debug);

    const auto& commandLine = Platform::CommandLine::Get();
    if(commandLine.HasArgument("ListTests"))
    {
        ListTests();
    }
    else if(const auto& outputPath = commandLine.GetArgumentValue("WriteTests"))
    {
        return DiscoverTests(*outputPath);
    }
    else if(const auto& testQuery = commandLine.GetArgumentValue("RunTests"))
    {
        return RunTests(*testQuery);
    }

    return RunAllTests();
}

void TestsApplication::ListTests()
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

ExitCode TestsApplication::DiscoverTests(const String& outputPath)
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
        return ExitCode::DiscoverTestsFailed;
    }

    LOG_SUCCESS("Discovered tests written to: %s", *outputPath);
    return ExitCode::Success;
}

ExitCode TestsApplication::RunTests(const StringView& testQuery)
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
        return ExitCode::QueryTestsFailed;
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
        return ExitCode::TestsFailed;
    }

    LOG_SUCCESS("Test execution was successful");
    return ExitCode::Success;
}

ExitCode TestsApplication::RunAllTests()
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
        return ExitCode::TestsFailed;
    }

    LOG_SUCCESS("All test execution was successful");
    return ExitCode::Success;
}
