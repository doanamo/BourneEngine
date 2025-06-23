#include "Shared.hpp"

TEST_DEFINE("Common.Optional", "EmptyConstructor")
{
    Optional<int> opt;
    TEST_FALSE(opt.HasValue());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "ValueConstructor")
{
    // #todo: Use Test::ObjectGuard more extensively without having to define it.
    // Make it thread local and reset automatically when test cases begin.
    Optional opt(42);
    TEST_TRUE(opt.HasValue());
    TEST_TRUE(opt.GetValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "MoveConstructor")
{
    Optional opt1(42);
    Optional opt2(Move(opt1));
    TEST_TRUE(opt2.HasValue());
    TEST_TRUE(opt2.GetValue() == 42);
    TEST_FALSE(opt1.HasValue());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "CopyConstructor")
{
    Optional<int> opt1(42);
    Optional<int> opt2(opt1);
    TEST_TRUE(opt2.HasValue());
    TEST_TRUE(opt2.GetValue() == 42);
    TEST_TRUE(opt1.HasValue());
    TEST_TRUE(opt1.GetValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "AssignmentOperator")
{
    Optional opt1(42);
    Optional<int> opt2;
    opt2 = opt1;
    TEST_TRUE(opt2.HasValue());
    TEST_TRUE(opt2.GetValue() == 42);
    TEST_TRUE(opt1.HasValue());
    TEST_TRUE(opt1.GetValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "MoveOperator")
{
    Optional opt1(42);
    Optional<int> opt2;
    opt2 = Move(opt1);
    TEST_TRUE(opt2.HasValue());
    TEST_TRUE(opt2.GetValue() == 42);
    TEST_FALSE(opt1.HasValue());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "Unwrap")
{
    Optional<int> opt(42);
    int value = opt.Unwrap();
    TEST_TRUE(value == 42);
    TEST_FALSE(opt.HasValue());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "Reset")
{
    Optional<int> opt(42);
    TEST_TRUE(opt.HasValue());
    opt.Reset();
    TEST_FALSE(opt.HasValue());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "DereferenceOperator")
{
    Optional<int> opt(42);
    TEST_TRUE(opt.HasValue());
    TEST_TRUE(*opt == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "ConstDereferenceOperator")
{
    const Optional<int> opt(42);
    TEST_TRUE(opt.HasValue());
    TEST_TRUE(*opt == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "PointerDereferenceOperator")
{
    Test::Object object(42);
    Optional<Test::Object*> opt(&object);
    TEST_TRUE(opt.HasValue());
    TEST_TRUE(opt->GetControlValue() == 42);
    TEST_TRUE(*opt == &object);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Optional", "ConstPointerDereferenceOperator")
{
    const Test::Object object(42);
    Optional<const Test::Object*> opt(&object);
    TEST_TRUE(opt.HasValue());
    TEST_TRUE(opt->GetControlValue() == 42);
    TEST_TRUE(*opt == &object);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}
