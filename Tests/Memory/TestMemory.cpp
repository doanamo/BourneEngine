#include "Shared.hpp"
#include "TestMemory.hpp"

TestResult Memory::RunTests()
{
    LOG_INFO("Running Memory::RunTests...");

#ifdef ENABLE_MEMORY_STATS
    u64 baseAllocationCount = Memory::Stats::Get().GetAllocationCount();
    u64 baseAllocatedBytes = Memory::Stats::Get().GetAllocatedUsableBytes();
#endif

    // Test allocation
    {
        u32* value = Memory::Allocate<u32>();
        TEST_TRUE(value != nullptr);

        *value = 42;
        TEST_TRUE(*value == 42);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32));
    #endif

        Memory::Deallocate(value, 1);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
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

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 4);
    #endif

        Memory::Deallocate(values, 4);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
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

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32));
    #endif

        values = Memory::Reallocate(values, 8, 1);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 1, 8));

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 8);
    #endif

        values = Memory::Reallocate(values, 64, 8);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 8, 64));

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 64);
    #endif

        values = Memory::Reallocate(values, 1024, 64);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 64, 1024));

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 1024);
    #endif

        values = Memory::Reallocate(values, 4, 1024);
        TEST_TRUE(values != nullptr);
        TEST_SUCCESS(ValidateAssign(values, 1024, 4));

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u32) * 4);
    #endif

        Memory::Deallocate(values, 4);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    // Test trivial construction
    {
        u64* trivial = Memory::Allocate<u64>();
        TEST_TRUE(trivial != nullptr);

        Memory::Construct(trivial, 42);
        TEST_TRUE(*trivial == 42);

        Memory::Destruct(trivial);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u64));
    #endif

        Memory::Deallocate(trivial, 1);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    // Test object construction
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

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(TestObject));
    #endif

        Memory::Deallocate(object, 1);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif

        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    // Test trivial array construction
    {
        u64* objects = Memory::Allocate<u64>(4);
        TEST_TRUE(objects != nullptr);

        Memory::ConstructRange(objects, objects + 4, 42);

        for(u32 i = 0; i < 4; i++)
        {
            TEST_TRUE(objects[i] == 42);
        }

        Memory::DestructRange(objects, objects + 4);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(u64) * 4);
    #endif

        Memory::Deallocate(objects, 4);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
    #endif
    }

    // Test object array construction
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

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(TestObject) * 4);
    #endif

        Memory::Deallocate(objects, 4);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
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

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount + 1);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes + sizeof(TestObject));
    #endif

        Memory::Delete(object);

    #ifdef ENABLE_MEMORY_STATS
        TEST_TRUE(Memory::Stats::Get().GetAllocationCount() == baseAllocationCount);
        TEST_TRUE(Memory::Stats::Get().GetAllocatedUsableBytes() == baseAllocatedBytes);
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
