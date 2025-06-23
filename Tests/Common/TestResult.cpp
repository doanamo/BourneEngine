#include "Shared.hpp"

enum class ResultError
{
    Unknown,
};

TEST_DEFINE("Common.Result", "Success")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    using ResultType = Result<Test::Object, ResultError>;

    auto successFunction = []() -> ResultType
    {
        return ResultType::Success(Test::Object(42));
    };

    ResultType successResult = successFunction();
    TEST_TRUE(successResult);
    TEST_TRUE(successResult.IsSuccess());
    TEST_FALSE(successResult.IsFailure());
    TEST_TRUE(successResult.Get().GetControlValue() == 42);
    TEST_TRUE(successResult.GetSuccess().GetControlValue() == 42);

    const ResultType& constSuccessType = successResult;
    TEST_TRUE(constSuccessType);
    TEST_TRUE(constSuccessType.IsSuccess());
    TEST_FALSE(constSuccessType.IsFailure());
    TEST_TRUE(constSuccessType.Get().GetControlValue() == 42);
    TEST_TRUE(constSuccessType.GetSuccess().GetControlValue() == 42);

    const Test::Object success = successResult.UnwrapSuccess();
    TEST_TRUE(success.GetControlValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    TEST_TRUE(objectGuard.ValidateTotalCounts(3, 1, 0, 2));
}

TEST_DEFINE("Common.Result", "Failure")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    using ResultType = Result<Test::Object, ResultError>;

    auto failureFunction = []() -> ResultType
    {
        return ResultType::Failure(ResultError::Unknown);
    };

    ResultType failureResult = failureFunction();
    TEST_FALSE(failureResult);
    TEST_FALSE(failureResult.IsSuccess());
    TEST_TRUE(failureResult.IsFailure());
    TEST_TRUE(failureResult.GetFailure() == ResultError::Unknown);

    const ResultType& constFailureType = failureResult;
    TEST_FALSE(constFailureType);
    TEST_FALSE(constFailureType.IsSuccess());
    TEST_TRUE(constFailureType.IsFailure());
    TEST_TRUE(constFailureType.GetFailure() == ResultError::Unknown);

    const ResultError error = failureResult.UnwrapFailure();
    TEST_TRUE(error == ResultError::Unknown);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    TEST_TRUE(objectGuard.ValidateTotalCounts(0, 0, 0, 0));
}
