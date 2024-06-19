#include "Shared.hpp"

namespace Common
{
    extern TestResult TestUtility();
}

TestResult TestCommon()
{
    TEST_SUCCESS(Common::TestUtility());
    return TestResult::Success;
}
