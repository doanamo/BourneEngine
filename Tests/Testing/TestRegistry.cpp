#include "Shared.hpp"
#include "TestRegistry.hpp"

Array<Test::Entry> Test::Registry::m_tests;

void Test::Registry::Register(const StringView& name, const FunctionPtr function)
{
    m_tests.Add(name, function);
}

Test::Registrar::Registrar(const StringView& name, const FunctionPtr function)
{
    Registry::Register(name, function);
}
