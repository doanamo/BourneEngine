#include "Shared.hpp"

TEST_DEFINE("Common.Optional")
{
    // Test default constructor
    {
        // #todo: Instead of defining memory guard manually per each test, each test case should define it by itself.
        // Or there should be a thread local global that can be accessed at any time, and which is reset on each test case.
        Test::MemoryGuard memoryGuard;

        Optional<int> opt;
        TEST_FALSE(opt.HasValue());

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test value constructor
    {
        Test::MemoryGuard memoryGuard;

        // #todo: Use Test::ObjectGuard more extensively without having to define it.
        // Make it thread local and reset automatically when test cases begin.
        Optional opt(42);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(opt.GetValue() == 42);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test move constructor
    {
        Test::MemoryGuard memoryGuard;

        Optional opt1(42);
        Optional opt2(Move(opt1));
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_FALSE(opt1.HasValue());

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test copy constructor
    {
        Test::MemoryGuard memoryGuard;

        Optional<int> opt1(42);
        Optional<int> opt2(opt1);
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_TRUE(opt1.HasValue());
        TEST_TRUE(opt1.GetValue() == 42);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test assignment operator
    {
        Test::MemoryGuard memoryGuard;

        Optional opt1(42);
        Optional<int> opt2;
        opt2 = opt1;
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_TRUE(opt1.HasValue());
        TEST_TRUE(opt1.GetValue() == 42);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test move assignment operator
    {
        Test::MemoryGuard memoryGuard;

        Optional opt1(42);
        Optional<int> opt2;
        opt2 = Move(opt1);
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_FALSE(opt1.HasValue());

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test unwrap function
    {
        Test::MemoryGuard memoryGuard;

        Optional<int> opt(42);
        int value = opt.Unwrap();
        TEST_TRUE(value == 42);
        TEST_FALSE(opt.HasValue());

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test reset function
    {
        Test::MemoryGuard memoryGuard;

        Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        opt.Reset();
        TEST_FALSE(opt.HasValue());

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test dereference operator
    {
        Test::MemoryGuard memoryGuard;

        Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(*opt == 42);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test const dereference operator
    {
        Test::MemoryGuard memoryGuard;

        const Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(*opt == 42);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test pointer dereference operator
    {
        Test::MemoryGuard memoryGuard;

        Test::Object object(42);
        Optional<Test::Object*> opt(&object);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(opt->GetControlValue() == 42);
        TEST_TRUE(*opt == &object);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    // Test const pointer dereference operator
    {
        Test::MemoryGuard memoryGuard;

        const Test::Object object(42);
        Optional<const Test::Object*> opt(&object);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(opt->GetControlValue() == 42);
        TEST_TRUE(*opt == &object);

        TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    }

    return Test::Result::Success;
}