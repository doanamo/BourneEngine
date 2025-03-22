#include "Shared.hpp"
#include "TestRegistry.hpp"

Test::Registry& Test::Registry::Get()
{
    static Test::Registry instance;
    return instance;
}

void Test::Registry::Register(const StringView& name, const FunctionPtr function)
{
    m_tests.Add(name, function);
}

Test::Registrar::Registrar(const StringView& name, const FunctionPtr function)
{
    Registry::Get().Register(name, function);
}
