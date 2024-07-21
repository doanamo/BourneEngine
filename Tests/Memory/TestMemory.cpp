#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::RunTests()
{
#ifndef CONFIG_RELEASE
    u64 baseAllocationCount = DefaultAllocator::GetAllocationCount();
    u64 baseAllocatedBytes = DefaultAllocator::GetAllocatedUsableBytes();
#endif

    // Test allocation
    {
        u32* value = Memory::Allocate<u32>();
        TEST_TRUE(value != nullptr);

        *value = 42;
        TEST_TRUE(*value == 42);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32));
    #endif

        Memory::Deallocate(value);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    // Test array allocation
    {
        u32* values = Memory::Allocate<u32>(4);
        TEST_TRUE(values != nullptr);

        values[0] = 1;
        values[1] = 2;
        values[2] = 3;
        values[3] = 4;

        TEST_TRUE(values[0] == 1);
        TEST_TRUE(values[1] == 2);
        TEST_TRUE(values[2] == 3);
        TEST_TRUE(values[3] == 4);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 4);
    #endif

        Memory::Deallocate(values);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
     }

    // Test reallocation
    {
        auto ValidateAssign = [](u32* values, u64 previousSize, u64 newSize) -> TestResult
        {
            for(u32 i = 0; i < Min(previousSize, newSize); i++)
            {
                TEST_TRUE(values[i] == i + 1);
            }

            for(u32 i = previousSize; i < newSize; i++)
            {
                values[i] = i + 1;
            }

            return TestResult::Success;
        };

        u32* values = Memory::Allocate<u32>();
        TEST_TRUE(values != nullptr);

        *values = 1;
        TEST_TRUE(*values == 1);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32));
    #endif

        values = Memory::Reallocate(values, 8);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 1, 8));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 8);
    #endif

        values = Memory::Reallocate(values, 64);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 8, 64));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 64);
    #endif

        values = Memory::Reallocate(values, 1024);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 64, 1024));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 1024);
    #endif

        values = Memory::Reallocate(values, 4);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 1024, 4));

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 4);
    #endif

        Memory::Deallocate(values);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    // Test construction
    {
        TestObject::ResetGlobalCounters();

        TestObject* object = Memory::Allocate<TestObject>();
        TEST_TRUE(object != nullptr);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        Memory::Construct(object, 42);
        TEST_TRUE(object->GetControlValue() == 42);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 1);

        Memory::Destruct(object);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(TestObject));
    #endif

        Memory::Deallocate(object);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    // Test array construction
    {
        TestObject::ResetGlobalCounters();

        TestObject* objects = Memory::Allocate<TestObject>(4);
        TEST_TRUE(objects != nullptr);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        Memory::ConstructRange(objects, objects + 4, 42);

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

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(TestObject) * 4);
    #endif

        Memory::Deallocate(objects);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif

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
        TEST_TRUE(object != nullptr);
        TEST_TRUE(object->GetControlValue() == 42);

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 1);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(TestObject));
    #endif

        Memory::Delete(object);

    #ifndef CONFIG_RELEASE
        TEST_TRUE(DefaultAllocator::GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(DefaultAllocator::GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    TEST_SUCCESS(TestOperators());
    return TestResult::Success;
}
