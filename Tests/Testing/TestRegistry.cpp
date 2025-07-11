#include "Shared.hpp"
#include "TestRegistry.hpp"
#include "Common/Algorithms/Sorting.hpp"

Test::Result Test::Entry::Run() const
{
    ASSERT(runner);
    return runner();
}

bool Test::Registry::Setup()
{
    InsertionSort(m_groups.GetBeginPtr(), m_groups.GetEndPtr());
    InsertionSort(m_tests.GetBeginPtr(), m_tests.GetEndPtr());

    std::atexit([]()
    {
        // Free memory from static storage before the memory leak check at process exit.
        Test::Registry& testRegistry = Test::Registry::Get();
        testRegistry.m_groups = {};
        testRegistry.m_tests = {};
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
    Registry::Get().Register(group, name, runner);
}

Test::Result Test::Detail::RunTestFunction(const StringView& group, const StringView& name, FunctionPtr function)
{
#if ENABLE_LOGGER
    u64 currentWarningCount = Logger::GetWarningCount();
    u64 currentErrorCount = Logger::GetErrorCount();
#endif

    SetCurrentTestResult(Result::Success);
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
