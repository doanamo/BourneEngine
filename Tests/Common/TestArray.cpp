#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestArray()
{
    LOG_INFO("Running Common::TestArray...");
    Memory::StatsTracker memoryStatsTracker;

    // Test empty array
    {
        Array<u32> array;
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));

        TEST_TRUE(array.GetData() == nullptr);
        TEST_TRUE(array.GetCapacity() == 0);
        TEST_TRUE(array.GetCapacityBytes() == 0);
        TEST_TRUE(array.GetSize() == 0);
        TEST_TRUE(array.GetSizeBytes() == 0);
        TEST_TRUE(array.GetUnusedCapacity() == 0);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 0);

        const Array<u32>& constArray = array;
        TEST_TRUE(constArray.GetData() == nullptr);
    }

    // Test array reserve
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array;
        array.Reserve(5);
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, sizeof(TestObject) * 5));

        const void* allocatedData = array.GetData();
        TEST_TRUE(allocatedData != nullptr);

        TEST_TRUE(array.GetData() == allocatedData);
        TEST_TRUE(array.GetCapacity() == 5);
        TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 0);
        TEST_TRUE(array.GetSizeBytes() == 0);
        TEST_TRUE(array.GetUnusedCapacity() == 5);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

        const Array<TestObject>& constArray = array;
        TEST_TRUE(constArray.GetData() == allocatedData);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test array resize
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array;
        array.Resize(1, 69);
        array.Resize(2, 18);
        array.Resize(5, TestObject(42));
        array.Resize(4);
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, sizeof(TestObject) * 5));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 5);
        TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 4);
        TEST_TRUE(array.GetSizeBytes() == 4 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 1);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 1 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 3);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 6);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 2);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 4);
        TEST_TRUE(array[0].GetControlValue() == 69);
        TEST_TRUE(array[1].GetControlValue() == 18);
        TEST_TRUE(array[2].GetControlValue() == 42);
        TEST_TRUE(array[3].GetControlValue() == 42);

        const Array<TestObject>& constArray = array;
        TEST_TRUE(constArray.GetData() == array.GetData());
        TEST_TRUE(constArray[0].GetControlValue() == 69);
        TEST_TRUE(constArray[1].GetControlValue() == 18);
        TEST_TRUE(constArray[2].GetControlValue() == 42);
        TEST_TRUE(constArray[3].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 3);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 6);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 6);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test array add trivial
    {
        Array<u32> array;
        array.Add();
        array.Add(18);
        array.Add(42);
        array.Add(69);
        array.Add(77);
        array.Resize(32, 23);
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, sizeof(u32) * 32));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 32);
        TEST_TRUE(array.GetCapacityBytes() == 32 * sizeof(u32));
        TEST_TRUE(array.GetSize() == 32);
        TEST_TRUE(array.GetSizeBytes() == 32 * sizeof(u32));
        TEST_TRUE(array.GetUnusedCapacity() == 0);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 0 * sizeof(u32));
        TEST_TRUE(array[0] == 0);
        TEST_TRUE(array[1] == 18);
        TEST_TRUE(array[2] == 42);
        TEST_TRUE(array[3] == 69);
        TEST_TRUE(array[4] == 77);

        for(u32 i = 5; i < 32; ++i)
        {
            TEST_TRUE(array[i] == 23);
        }

        const Array<u32>& constArray = array;
        TEST_TRUE(constArray.GetData() == array.GetData());
        TEST_TRUE(constArray[0] == 0);
        TEST_TRUE(constArray[1] == 18);
        TEST_TRUE(constArray[2] == 42);
        TEST_TRUE(constArray[3] == 69);
        TEST_TRUE(constArray[4] == 77);

        for(u32 i = 5; i < 32; ++i)
        {
            TEST_TRUE(constArray[i] == 23);
        }
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));

    // Test array add object
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array;
        array.Add();
        array.Add(18);
        array.Add(TestObject(42));
        array.Add(69);
        array.Add(TestObject(77));
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, sizeof(TestObject) * 8));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 5);
        TEST_TRUE(array.GetSizeBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 3);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 2);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 7);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 2);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 5);
        TEST_TRUE(array[0].GetControlValue() == 0);
        TEST_TRUE(array[1].GetControlValue() == 18);
        TEST_TRUE(array[2].GetControlValue() == 42);
        TEST_TRUE(array[3].GetControlValue() == 69);
        TEST_TRUE(array[4].GetControlValue() == 77);

        const Array<TestObject>& constArray = array;
        TEST_TRUE(constArray.GetData() == array.GetData());
        TEST_TRUE(constArray[0].GetControlValue() == 0);
        TEST_TRUE(constArray[1].GetControlValue() == 18);
        TEST_TRUE(constArray[2].GetControlValue() == 42);
        TEST_TRUE(constArray[3].GetControlValue() == 69);
        TEST_TRUE(constArray[4].GetControlValue() == 77);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 2);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 7);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 7);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test array clear
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array;
        array.Resize(8);
        array.Clear();
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, 8* sizeof(TestObject)));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 0);
        TEST_TRUE(array.GetSizeBytes() == 0 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 8);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 8);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 8);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 8);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 8);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test array shrink to fit
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array;
        array.Reserve(8);
        array.Resize(3, 42);
        array.ShrinkToFit();
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, 3 * sizeof(TestObject)));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 3);
        TEST_TRUE(array.GetCapacityBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 3);
        TEST_TRUE(array.GetSizeBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 0);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 0 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 3);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 3);
        TEST_TRUE(array[0].GetControlValue() == 42);
        TEST_TRUE(array[1].GetControlValue() == 42);
        TEST_TRUE(array[2].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 3);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 3);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test array copy
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array1;
        Array<TestObject> array2;
        array1.Reserve(8);
        array1.Resize(3, 42);
        array2 = array1;
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(2, 12 * sizeof(TestObject)));

        TEST_TRUE(array1.GetData() != nullptr);
        TEST_TRUE(array1.GetCapacity() == 8);
        TEST_TRUE(array1.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array1.GetSize() == 3);
        TEST_TRUE(array1.GetSizeBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(array1.GetUnusedCapacity() == 5);
        TEST_TRUE(array1.GetUnusedCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(array2.GetData() != nullptr);
        TEST_TRUE(array2.GetCapacity() == 4);
        TEST_TRUE(array2.GetCapacityBytes() == 4 * sizeof(TestObject));
        TEST_TRUE(array2.GetSize() == 3);
        TEST_TRUE(array2.GetSizeBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(array2.GetUnusedCapacity() == 1);
        TEST_TRUE(array2.GetUnusedCapacityBytes() == 1 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 3);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 6);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 6);
        TEST_TRUE(array1[0].GetControlValue() == 42);
        TEST_TRUE(array1[1].GetControlValue() == 42);
        TEST_TRUE(array1[2].GetControlValue() == 42);
        TEST_TRUE(array2[0].GetControlValue() == 42);
        TEST_TRUE(array2[1].GetControlValue() == 42);
        TEST_TRUE(array2[2].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 3);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 6);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 6);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test array move
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array1;
        Array<TestObject> array2;
        array1.Reserve(8);
        array1.Resize(3, 42);
        array2 = std::move(array1);
        TEST_TRUE(memoryStatsTracker.ValidateAllocations(1, 8 * sizeof(TestObject)));

        TEST_TRUE(array1.GetData() == nullptr);
        TEST_TRUE(array1.GetCapacity() == 0);
        TEST_TRUE(array1.GetCapacityBytes() == 0);
        TEST_TRUE(array1.GetSize() == 0);
        TEST_TRUE(array1.GetSizeBytes() == 0);
        TEST_TRUE(array1.GetUnusedCapacity() == 0);
        TEST_TRUE(array1.GetUnusedCapacityBytes() == 0);
        TEST_TRUE(array2.GetData() != nullptr);
        TEST_TRUE(array2.GetCapacity() == 8);
        TEST_TRUE(array2.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array2.GetSize() == 3);
        TEST_TRUE(array2.GetSizeBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(array2.GetUnusedCapacity() == 5);
        TEST_TRUE(array2.GetUnusedCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 3);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 3);
        TEST_TRUE(array2[0].GetControlValue() == 42);
        TEST_TRUE(array2[1].GetControlValue() == 42);
        TEST_TRUE(array2[2].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStatsTracker.ValidateAllocations(0, 0));
    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 3);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 3);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    return TestResult::Success;
}
