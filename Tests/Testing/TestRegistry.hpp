#pragma once

#include "TestDefines.hpp"
#include "TestGuards.hpp"

namespace Test
{
    #define TEST_FUNCTION_ARGUMENTS Test::MemoryGuard& memoryGuard, Test::ObjectGuard& objectGuard
    using FunctionPtr = void (*)(TEST_FUNCTION_ARGUMENTS);
    using RunnerPtr = Result (*)();

    struct Entry
    {
        StringView group;
        StringView name;
        RunnerPtr runner = nullptr;

        Result Run() const;
    };

    class Registry
    {
        static Array<StringView> m_groups;
        static Array<Entry> m_tests;

    public:
        static void Register(const StringView& group, const StringView& name, RunnerPtr runner);

        static const Array<StringView>& GetGroups()
        {
            return m_groups;
        }

        static const Array<Entry>& GetTests()
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
        Result RunTestFunction(FunctionPtr function);
    }
}

#define TEST_DEFINE_PRIVATE(group, name, counter) \
    static void CONCAT(TestFunction, counter)(TEST_FUNCTION_ARGUMENTS); \
    static Test::Result CONCAT(TestRunner, counter)() \
    { \
        LOG_INFO("Running test: %s.%s", group, name); \
        return Test::Detail::RunTestFunction(CONCAT(TestFunction, counter)); \
    } \
    static Test::Registrar CONCAT(TestRegistrar, counter)(group, name, &CONCAT(TestRunner, counter)); \
    static void CONCAT(TestFunction, counter)(TEST_FUNCTION_ARGUMENTS)

#define TEST_DEFINE(group, name) \
    TEST_DEFINE_PRIVATE(group, name, __COUNTER__)
