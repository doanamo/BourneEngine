#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Platform/CommandLine.hpp"

class TestsApplication final : public Application
{
public:
    Config GetConfig() override;

    bool OnSetup() override;
    Optional<ExitCodes> OnRun() override;

private:
    void ListTests();
    ExitCodes DiscoverTests(const String& outputPath);
    ExitCodes RunTest(const StringView& testPath);
    ExitCodes RunTests(const StringView& testQuery);
    ExitCodes RunAllTests();
};

DEFINE_PRIMARY_APPLICATION("Bourne Engine Tests", TestsApplication);

Config TestsApplication::GetConfig()
{
    // #todo: Don't print time stamps for test output.
    Config config;
    config.logger.minimumSeverity = Logger::Severity::Warning;
    config.headless = true;
    return config;
}

bool TestsApplication::OnSetup()
{
    if(!Test::Registry::Get().Setup())
    {
        LOG_ERROR("Failed to setup test registry");
        return false;
    }

    return true;
}

Optional<ExitCodes> TestsApplication::OnRun()
{
    LOG_MINIMUM_SEVERITY_SCOPE(Logger::Severity::Debug);

    const auto& commandLine = Platform::CommandLine::Get();
    if(commandLine.HasArgument("ListTests"))
    {
        ListTests();
    }
    else if(const auto& outputPath = commandLine.GetArgumentValue("DiscoverTests"))
    {
        return DiscoverTests(*outputPath);
    }
    else if(const auto& testPath = commandLine.GetArgumentValue("RunTest"))
    {
        return RunTest(*testPath);
    }
    else if(const auto& testQuery = commandLine.GetArgumentValue("RunTests"))
    {
        return RunTests(*testQuery);
    }

    return RunAllTests();
}

void TestsApplication::ListTests()
{
    Test::Registry& testRegistry = Test::Registry::Get();
    LOG_INFO("Printing %lu available test(s) from %lu group(s):",
         testRegistry.GetTests().GetSize(),
         testRegistry.GetGroups().GetSize());

    LOG_NO_SOURCE_LINE_SCOPE();
    for(const Test::Entry& testEntry : testRegistry.GetTests())
    {
        LOG_INFO("  %.*s.%.*s", STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));
    }
}

ExitCodes TestsApplication::DiscoverTests(const String& outputPath)
{
    Test::Registry& testRegistry = Test::Registry::Get();
    LOG_INFO("Writing %llu discovered test(s) from %llu group(s)...",
        testRegistry.GetTests().GetSize(), testRegistry.GetTests().GetSize());

    HeapString builder;
    for (const Test::Entry& testEntry : testRegistry.GetTests())
    {
        builder.Append("add_test(\"%.*s.%.*s\" Tests [==[-RunTest=%.*s.%.*s]==])\n",
            STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name),
            STRING_VIEW_PRINTF_ARG(testEntry.group), STRING_VIEW_PRINTF_ARG(testEntry.name));
    }

    if(!WriteStringToFileIfDifferent(outputPath, builder))
    {
        LOG_ERROR("Failed to write discovered tests");
        return ExitCodes::DiscoverTestsFailed;
    }

    LOG_SUCCESS("Discovered tests written to: %s", *outputPath);
    return ExitCodes::Success;
}

ExitCodes TestsApplication::RunTest(const StringView& testPath)
{
    const Test::Entry* testEntry = Test::Registry::Get().GetTests().FindPredicate(
        [&testPath](const Test::Entry& entry)
        {
            // #todo: Replace need to concatenate test path, have a method for that.
            auto fullTestName = InlineString<128>::Format("%.*s.%.*s",
                STRING_VIEW_PRINTF_ARG(entry.group), STRING_VIEW_PRINTF_ARG(entry.name));
            return fullTestName == testPath;
        });

    if (!testEntry)
    {
        LOG_ERROR("Failed to find test with \"%.*s\" path", STRING_VIEW_PRINTF_ARG(testPath));
        return ExitCodes::QueryTestsFailed;
    }

    if (testEntry->Run() != Test::Result::Success)
    {
        LOG_ERROR("Test execution was unsuccessful");
        return ExitCodes::RunTestsFailed;
    }

    LOG_SUCCESS("Test execution was successful");
    return ExitCodes::Success;
}

ExitCodes TestsApplication::RunTests(const StringView& testQuery)
{
    Array<const Test::Entry*> foundTests;
    for(const Test::Entry& testEntry : Test::Registry::Get().GetTests())
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
        return ExitCodes::QueryTestsFailed;
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
        LOG_ERROR("Test execution was unsuccessful due to %u failure(s)", testsFailed);
        return ExitCodes::RunTestsFailed;
    }

    LOG_SUCCESS("Test execution was successful");
    return ExitCodes::Success;
}

ExitCodes TestsApplication::RunAllTests()
{
    Test::Registry& testRegistry = Test::Registry::Get();
    LOG_INFO("Running all %lu test(s) from %lu group(s)...",
         testRegistry.GetTests().GetSize(),
         testRegistry.GetGroups().GetSize());

    u32 testsFailed = 0;
    for(const Test::Entry& testEntry : testRegistry.GetTests())
    {
        if (testEntry.Run() != Test::Result::Success)
        {
            ++testsFailed;
        }
    }

    if(testsFailed > 0)
    {
        LOG_ERROR("Execution of all tests was unsuccessful due to %u failure(s)", testsFailed);;
        return ExitCodes::RunTestsFailed;
    }

    LOG_SUCCESS("Execution of all tests was successful");
    return ExitCodes::Success;
}
