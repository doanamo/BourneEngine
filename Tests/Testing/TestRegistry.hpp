#pragma once

#include "TestDefines.hpp"

namespace Test
{
    using FunctionPtr = void (*)(void);

    struct Entry
    {
        StringView group;
        StringView name;
        FunctionPtr function;
    };

    class Registry
    {
        static Array<StringView> m_groups;
        static Array<Entry> m_tests;

    public:
        static void Register(const StringView& group, const StringView& name, FunctionPtr function);

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
        Registrar(const StringView& group, const StringView& name, FunctionPtr function);
    };
}

// #todo: When "Running test" log message is printed, it points to Main.
// Make a thin function wrapper in define that will print executed test log with proper source code link.
#define TEST_DEFINE_PRIVATE(group, name, counter) \
    static void CONCAT(TestFunction, counter)(); \
    static Test::Registrar CONCAT(TestRegistrar, counter)(group, name, &CONCAT(TestFunction, counter)); \
    static void CONCAT(TestFunction, counter)()

#define TEST_DEFINE(group, name) \
    TEST_DEFINE_PRIVATE(group, name, __COUNTER__)
