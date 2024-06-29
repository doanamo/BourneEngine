#include "Shared.hpp"
#include "Engine/Engine.hpp"
#include "Common/TestCommon.hpp"

TestResult RunTests()
{
    TEST_SUCCESS(Common::TestCommon());
    return TestResult::Success;
}

int main()
{
    Engine::Setup();

    if(RunTests() != TestResult::Success)
    {
        LOG_ERROR("Test execution has failed");
        return -1;
    }

    LOG_SUCCESS("All tests passed successfully");
    return 0;
}
