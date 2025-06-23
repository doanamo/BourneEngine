#include "Shared.hpp"
#include "TestRegistry.hpp"

Array<StringView> Test::Registry::m_groups;
Array<Test::Entry> Test::Registry::m_tests;

Test::Result Test::Entry::Run() const
{
    ASSERT(runner);
    return runner();
}

void Test::Registry::Register(const StringView& group, const StringView& name, const RunnerPtr runner)
{
    m_groups.AddUnique(group);
    // #todo: Check for duplicate group and name combinations.
    m_tests.Add(group, name, runner);
}

Test::Registrar::Registrar(const StringView& group, const StringView& name, const RunnerPtr runner)
{
    Registry::Register(group, name, runner);
}

Test::Result Test::Detail::RunTestFunction(const StringView& group, const StringView& name, FunctionPtr function)
{
    {
        SetCurrentTestResult(Result::Success);
        MemoryGuard memoryGuard;
        ObjectGuard objectGuard;

        ASSERT(function);
        function(memoryGuard, objectGuard);
    }

    Result result = GetCurrentTestResult();
    if(result != Result::Success)
    {
        LOG_ERROR("Test \" %.*s.%.*s\" failed!", STRING_VIEW_PRINTF_ARG(group), STRING_VIEW_PRINTF_ARG(name));
    }

    return result;
}
