#include "Shared.hpp"
#include "TestDefines.hpp"

static Test::Result g_currentTestResult = Test::Result::Success;

void Test::SetCurrentTestResult(Result result)
{
    g_currentTestResult = result;
}

Test::Result Test::GetCurrentTestResult()
{
    return g_currentTestResult;
}
