#pragma once

#include "Common/Utility/Singleton.hpp"
#include "TestDefines.hpp"
#include "TestGuards.hpp"

namespace Test
{
    using FunctionPtr = void (*)(MemoryGuard& memoryGuard, ObjectGuard& objectGuard);
    using RunnerPtr = Result (*)();

    struct Entry
    {
        StringView group;
        StringView name;
        RunnerPtr runner = nullptr;

        Result Run() const;

        bool operator<(const Entry& other) const
        {
            // Order by group only and keep definition ordering.
            return group < other.group;
        }
    };

    class Registry : public Singleton<Registry>
    {
        Array<StringView> m_groups;
        Array<Entry> m_tests;

    public:
        bool Setup();
        void Register(const StringView& group, const StringView& name, RunnerPtr runner);

        const Array<StringView>& GetGroups()
        {
            return m_groups;
        }

        const Array<Entry>& GetTests()
        {
            return m_tests;
        }
    };

    class Registrar
    {
    public:
        Registrar(const StringView& group, const StringView& name, RunnerPtr runner);
    };

    namespace Detail
    {
        Result RunTestFunction(const StringView& group, const StringView& name, FunctionPtr function);
    }
}

#define TEST_DEFINE_PRIVATE(group, name, counter) \
    static void CONCAT(TestFunction, counter)(Test::MemoryGuard& memoryGuard, Test::ObjectGuard& objectGuard); \
    static Test::Result CONCAT(TestRunner, counter)() \
    { \
        LOG_INFO("Running \"%s.%s\" test", group, name); \
        return Test::Detail::RunTestFunction(group, name, CONCAT(TestFunction, counter)); \
    } \
    static Test::Registrar CONCAT(TestRegistrar, counter)(group, name, &CONCAT(TestRunner, counter)); \
    static void CONCAT(TestFunction, counter)(Test::MemoryGuard& memoryGuard, Test::ObjectGuard& objectGuard)

#define TEST_DEFINE(group, name) \
    TEST_DEFINE_PRIVATE(group, name, __COUNTER__)
