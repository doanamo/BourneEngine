#include "Shared.hpp"
#include "TestRegistry.hpp"

Array<StringView> Test::Registry::m_groups;
Array<Test::Entry> Test::Registry::m_tests;

Test::Result Test::Entry::Run() const
{
    ASSERT(runner);
    return runner();
}

bool Test::Registry::Setup()
{
    // #todo: Sort discovered test groups (not names which should remain in order of definition).

    std::atexit([]()
    {
        // Free memory from static storage before the memory leak check at process exit.
        m_groups = {};
        m_tests = {};
    });

    return true;
}

void Test::Registry::Register(const StringView& group, const StringView& name, const RunnerPtr runner)
{
    m_groups.AddUnique(group);

    bool exists = m_tests.ContainsPredicate(
        [&group, &name](const Test::Entry& entry)
        {
            return entry.group == group && entry.name == name;
        });

    ASSERT_ALWAYS(!exists, "Test with path \"%.*s.%.*s\" is already registered!",
        STRING_VIEW_PRINTF_ARG(group), STRING_VIEW_PRINTF_ARG(name));

    m_tests.Add(group, name, runner);
}

Test::Registrar::Registrar(const StringView& group, const StringView& name, const RunnerPtr runner)
{
    Registry::Register(group, name, runner);
}

Test::Result Test::Detail::RunTestFunction(const StringView& group, const StringView& name, FunctionPtr function)
{
#if ENABLE_LOGGER
    u64 currentWarningCount = Logger::GetWarningCount();
    u64 currentErrorCount = Logger::GetErrorCount();
#endif

    {
        MemoryGuard memoryGuard;
        ObjectGuard objectGuard;

        ASSERT(function);
        function(memoryGuard, objectGuard);
    }

    Result result = GetCurrentTestResult();

#if ENABLE_LOGGER
    u64 warningCount = Logger::GetWarningCount() - currentWarningCount;
    u64 errorCount = Logger::GetErrorCount() - currentErrorCount;
    if(errorCount > 0 || warningCount > 0)
    {
        result = Result::Failure;
    }
#endif

    if(result != Result::Success)
    {
        LOG_ERROR("Test \"%.*s.%.*s\" failed!", STRING_VIEW_PRINTF_ARG(group), STRING_VIEW_PRINTF_ARG(name));
    }

    return result;
}
