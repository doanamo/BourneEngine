#include "Shared.hpp"
#include "TestCommon.hpp"
#include "Memory/TestMemoryStats.hpp"

enum class ResultError
{
    Unknown,
};

Test::Result Common::TestResult()
{
    LOG_INFO("Running Common::TestResult...");
    const Memory::TestStats memoryStats;

    // Test result with success and failure types
    TestObject::ResetGlobalCounters();

    {
        using ResultType = Result<TestObject, ResultError>;

        auto successFunction = []() -> ResultType
        {
            return Success(TestObject(42));
        };

        auto failureFunction = []() -> ResultType
        {
            return Failure(ResultError::Unknown);
        };

        ResultType successResult = successFunction();
        TEST_TRUE(successResult);
        TEST_TRUE(successResult.IsSuccess());
        TEST_FALSE(successResult.IsFailure());
        TEST_TRUE(successResult.Get().GetControlValue() == 42);
        TEST_TRUE(successResult.GetSuccess().GetControlValue() == 42);

        ResultType failureResult = failureFunction();
        TEST_FALSE(failureResult);
        TEST_FALSE(failureResult.IsSuccess());
        TEST_TRUE(failureResult.IsFailure());
        TEST_TRUE(failureResult.GetFailure() == ResultError::Unknown);

        const ResultType& constSuccessType = successResult;
        TEST_TRUE(constSuccessType);
        TEST_TRUE(constSuccessType.IsSuccess());
        TEST_FALSE(constSuccessType.IsFailure());
        TEST_TRUE(constSuccessType.Get().GetControlValue() == 42);
        TEST_TRUE(constSuccessType.GetSuccess().GetControlValue() == 42);

        const ResultType& constFailureType = failureResult;
        TEST_FALSE(constFailureType);
        TEST_FALSE(constFailureType.IsSuccess());
        TEST_TRUE(constFailureType.IsFailure());
        TEST_TRUE(constFailureType.GetFailure() == ResultError::Unknown);

        const TestObject success = successResult.UnwrapSuccess();
        TEST_TRUE(success.GetControlValue() == 42);

        const ResultError error = failureResult.UnwrapFailure();
        TEST_TRUE(error == ResultError::Unknown);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 3);
    TEST_TRUE(TestObject::GetConstructCount() == 4);
    TEST_TRUE(TestObject::GetDestructCount() == 4);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    return Test::Result::Success;
}
