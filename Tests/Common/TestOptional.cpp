#include "Shared.hpp"

TEST_DEFINE("Common.Optional")
{
    const Test::MemoryStats memoryStats;

    // Test default constructor
    {
        Optional<int> opt;
        TEST_FALSE(opt.HasValue());
    }

    // Test value constructor
    {
        Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(opt.GetValue() == 42);
    }

    // Test move constructor
    {
        Optional<int> opt1(42);
        Optional<int> opt2(std::move(opt1));
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_FALSE(opt1.HasValue());
    }

    // Test copy constructor
    {
        Optional<int> opt1(42);
        Optional<int> opt2(opt1);
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_TRUE(opt1.HasValue());
        TEST_TRUE(opt1.GetValue() == 42);
    }

    // Test assignment operator
    {
        Optional<int> opt1(42);
        Optional<int> opt2;
        opt2 = opt1;
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_TRUE(opt1.HasValue());
        TEST_TRUE(opt1.GetValue() == 42);
    }

    // Test move assignment operator
    {
        Optional<int> opt1(42);
        Optional<int> opt2;
        opt2 = std::move(opt1);
        TEST_TRUE(opt2.HasValue());
        TEST_TRUE(opt2.GetValue() == 42);
        TEST_FALSE(opt1.HasValue());
    }

    // Test unwrap function
    {
        Optional<int> opt(42);
        int value = opt.Unwrap();
        TEST_TRUE(value == 42);
        TEST_FALSE(opt.HasValue());
    }

    // Test reset function
    {
        Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        opt.Reset();
        TEST_FALSE(opt.HasValue());
    }

    // Test dereference operator
    {
        Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(*opt == 42);
    }

    // Test const dereference operator
    {
        const Optional<int> opt(42);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(*opt == 42);
    }

    // Test pointer dereference operator
    {
        Test::Object object(42);
        Optional<Test::Object*> opt(&object);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(opt->GetControlValue() == 42);
        TEST_TRUE(*opt == &object);
    }

    // Test const pointer dereference operator
    {
        const Test::Object object(42);
        Optional<const Test::Object*> opt(&object);
        TEST_TRUE(opt.HasValue());
        TEST_TRUE(opt->GetControlValue() == 42);
        TEST_TRUE(*opt == &object);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    return Test::Result::Success;
}