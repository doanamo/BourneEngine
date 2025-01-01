#pragma once
namespace Common
{
    Test::Result RunTests();
    Test::Result TestUtility();
    Test::Result TestScopeGuard();
    Test::Result TestResult();
    Test::Result TestOptional();
    Test::Result TestFunction();
    Test::Result TestUniquePtr();
    Test::Result TestArray();
    Test::Result TestStringView();
    Test::Result TestString();
} // namespace Common
