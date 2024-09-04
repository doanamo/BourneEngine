#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::RunTests()
{
    TEST_SUCCESS(TestMemory());
    TEST_SUCCESS(TestOperators());
    return TestResult::Success;
}

TestResult Memory::TestMemory()
{
    LOG_INFO("Running Memory::TestMemory...");
    Memory::ScopedStats memoryStats;

    // Test allocation
    {
        u32* value = Memory::Allocate<u32>();
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32)));
        TEST_TRUE(value != nullptr);

        *value = 42;
        TEST_TRUE(*value == 42);

        Memory::Deallocate(value, 1);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test array allocation
    {
        u32* values = Memory::Allocate<u32>(4);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 4));
        TEST_TRUE(values != nullptr);

        values[0] = 1;
        values[1] = 2;
        values[2] = 3;
        values[3] = 4;

        TEST_TRUE(values[0] == 1);
        TEST_TRUE(values[1] == 2);
        TEST_TRUE(values[2] == 3);
        TEST_TRUE(values[3] == 4);

        Memory::Deallocate(values, 4);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
     }

    // Test reallocation
    {
        auto ValidateAssign = [](u32* values, u64 previousSize, u64 newSize) -> bool
        {
            for(u32 i = 0; i < Min(previousSize, newSize); i++)
            {
                if(values[i] != i + 1)
                {
                    return false;
                }
            }

            for(u32 i = previousSize; i < newSize; i++)
            {
                values[i] = i + 1;
            }

            return true;
        };

        u32* values = Memory::Allocate<u32>();
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32)));
        TEST_TRUE(values != nullptr);

        *values = 1;
        TEST_TRUE(*values == 1);

        values = Memory::Reallocate(values, 8, 1);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 8));
        TEST_TRUE(values != nullptr);
        TEST_TRUE(ValidateAssign(values, 1, 8));

        values = Memory::Reallocate(values, 64, 8);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 64));
        TEST_TRUE(values != nullptr);
        TEST_TRUE(ValidateAssign(values, 8, 64));

        values = Memory::Reallocate(values, 1024, 64);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 1024));
        TEST_TRUE(values != nullptr);
        TEST_TRUE(ValidateAssign(values, 64, 1024));

        values = Memory::Reallocate(values, 4, 1024);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 4));
        TEST_TRUE(values != nullptr);
        TEST_TRUE(ValidateAssign(values, 1024, 4));

        Memory::Deallocate(values, 4);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test trivial construction
    {
        u64* trivial = Memory::Allocate<u64>();
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u64)));
        TEST_TRUE(trivial != nullptr);

        Memory::Construct(trivial, 42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u64)));
        TEST_TRUE(*trivial == 42);

        Memory::Destruct(trivial);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u64)));

        Memory::Deallocate(trivial, 1);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test object construction
    {
        TestObject::ResetGlobalCounters();

        TestObject* object = Memory::Allocate<TestObject>();
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject)));
        TEST_TRUE(object != nullptr);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        Memory::Construct(object, 42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject)));
        TEST_TRUE(object->GetControlValue() == 42);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 1);

        Memory::Destruct(object);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject)));

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        Memory::Deallocate(object, 1);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    // Test trivial array construction
    {
        u64* objects = Memory::Allocate<u64>(4);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u64) * 4));
        TEST_TRUE(objects != nullptr);

        Memory::ConstructRange(objects, objects + 4, 42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u64) * 4));

        for(u32 i = 0; i < 4; i++)
        {
            TEST_TRUE(objects[i] == 42);
        }

        Memory::DestructRange(objects, objects + 4);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u64) * 4));

        Memory::Deallocate(objects, 4);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    // Test object array construction
    {
        TestObject::ResetGlobalCounters();

        TestObject* objects = Memory::Allocate<TestObject>(4);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject) * 4));
        TEST_TRUE(objects != nullptr);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        Memory::ConstructRange(objects, objects + 4, 42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject) * 4));

        for(u32 i = 0; i < 4; i++)
        {
            TEST_TRUE(objects[i].GetControlValue() == 42);
        }

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 4);

        Memory::DestructRange(objects, objects + 4);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject) * 4));

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        Memory::Deallocate(objects, 4);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    // Test new
    {
        TestObject::ResetGlobalCounters();

        TestObject* object = Memory::New<TestObject>(42);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(TestObject)));
        TEST_TRUE(object != nullptr);
        TEST_TRUE(object->GetControlValue() == 42);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 1);

        Memory::Delete(object);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    return TestResult::Success;
}
