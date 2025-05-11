#pragma once

#include "TestDefines.hpp"

namespace Test
{
    using FunctionPtr = Result (*)(void);

    struct Entry
    {
        StringView name;
        FunctionPtr function;
    };

    class Registry
    {
        static Array<Entry> m_tests;

    public:
        static void Register(const StringView& name, FunctionPtr function);

        static const Array<Entry>& GetTests()
        {
            return m_tests;
        }

        static u64 GetTestCount()
        {
            return m_tests.GetSize();
        }
    };

    class Registrar
    {
    public:
        Registrar(const StringView& name, FunctionPtr function);
    };
}

#define TEST_DEFINE_PRIVATE(name, counter) \
    static Test::Result CONCAT(TestFunction, counter)(); \
    static Test::Registrar CONCAT(TestRegistrar, counter)(name, &CONCAT(TestFunction, counter)); \
    static Test::Result CONCAT(TestFunction, counter)()

#define TEST_DEFINE(name) \
    TEST_DEFINE_PRIVATE(name, __COUNTER__)
