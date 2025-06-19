#include "Shared.hpp"
#include "TestRegistry.hpp"

Array<StringView> Test::Registry::m_groups;
Array<Test::Entry> Test::Registry::m_tests;

void Test::Registry::Register(const StringView& group, const StringView& name, const FunctionPtr function)
{
    m_groups.AddUnique(group);
    // #todo: Check for duplicate group and name combinations.
    m_tests.Add(group, name, function);
}

Test::Registrar::Registrar(const StringView& group, const StringView& name, const FunctionPtr function)
{
    Registry::Register(group, name, function);
}
