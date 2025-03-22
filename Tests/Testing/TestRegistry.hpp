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
        Array<Entry> m_tests;

    public:
        static Registry& Get();

        void Register(const StringView& name, FunctionPtr function);

        const Array<Entry>& GetTests() const
        {
            return m_tests;
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
