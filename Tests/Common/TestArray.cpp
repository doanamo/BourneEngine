#include "Shared.hpp"
#include "TestCommon.hpp"

Test::Result Common::TestArray()
{
    LOG_INFO("Running Common::TestArray...");
    const Test::MemoryStats memoryStats;

    // Test empty array
    {
        Array<u32> array;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

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
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array;
        array.Reserve(5);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object) * 5));

        const void* allocatedData = array.GetData();
        TEST_TRUE(allocatedData != nullptr);

        TEST_TRUE(array.GetData() == allocatedData);
        TEST_TRUE(array.GetCapacity() == 5);
        TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(Test::Object));
        TEST_TRUE(array.GetSize() == 0);
        TEST_TRUE(array.GetSizeBytes() == 0);
        TEST_TRUE(array.GetUnusedCapacity() == 5);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 5 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 0);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 0);
        TEST_TRUE(Test::Object::GetDestructCount() == 0);
        TEST_TRUE(Test::Object::GetInstanceCount() == 0);

        const Array<Test::Object>& constArray = array;
        TEST_TRUE(constArray.GetData() == allocatedData);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 0);
    TEST_TRUE(Test::Object::GetDestructCount() == 0);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array resize
    {
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array;
        array.Resize(1, 69);
        array.Resize(2, 18);
        array.Resize(5, Test::Object(42));
        array.Resize(4);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object) * 5));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 5);
        TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(Test::Object));
        TEST_TRUE(array.GetSize() == 4);
        TEST_TRUE(array.GetSizeBytes() == 4 * sizeof(Test::Object));
        TEST_TRUE(array.GetUnusedCapacity() == 1);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 1 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 3);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 6);
        TEST_TRUE(Test::Object::GetDestructCount() == 2);
        TEST_TRUE(Test::Object::GetInstanceCount() == 4);
        TEST_TRUE(array[0].GetControlValue() == 69);
        TEST_TRUE(array[1].GetControlValue() == 18);
        TEST_TRUE(array[2].GetControlValue() == 42);
        TEST_TRUE(array[3].GetControlValue() == 42);

        const Array<Test::Object>& constArray = array;
        TEST_TRUE(constArray.GetData() == array.GetData());
        TEST_TRUE(constArray[0].GetControlValue() == 69);
        TEST_TRUE(constArray[1].GetControlValue() == 18);
        TEST_TRUE(constArray[2].GetControlValue() == 42);
        TEST_TRUE(constArray[3].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 3);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 6);
    TEST_TRUE(Test::Object::GetDestructCount() == 6);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array add trivial
    {
        Array<u32> array;
        array.Add();
        array.Add(18);
        array.Add(42);
        array.Add(69);
        array.Add(77);
        array.Resize(32, 23);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(u32) * 32));

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

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

    // Test array add object
    {
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array;
        array.Add();
        array.Add(18);
        array.Add(Test::Object(42));
        array.Add(69);
        array.Add(Test::Object(77));
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object) * 8));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(Test::Object));
        TEST_TRUE(array.GetSize() == 5);
        TEST_TRUE(array.GetSizeBytes() == 5 * sizeof(Test::Object));
        TEST_TRUE(array.GetUnusedCapacity() == 3);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 0);
        TEST_TRUE(Test::Object::GetMoveCount() == 2);
        TEST_TRUE(Test::Object::GetConstructCount() == 7);
        TEST_TRUE(Test::Object::GetDestructCount() == 2);
        TEST_TRUE(Test::Object::GetInstanceCount() == 5);
        TEST_TRUE(array[0].GetControlValue() == 0);
        TEST_TRUE(array[1].GetControlValue() == 18);
        TEST_TRUE(array[2].GetControlValue() == 42);
        TEST_TRUE(array[3].GetControlValue() == 69);
        TEST_TRUE(array[4].GetControlValue() == 77);

        const Array<Test::Object>& constArray = array;
        TEST_TRUE(constArray.GetData() == array.GetData());
        TEST_TRUE(constArray[0].GetControlValue() == 0);
        TEST_TRUE(constArray[1].GetControlValue() == 18);
        TEST_TRUE(constArray[2].GetControlValue() == 42);
        TEST_TRUE(constArray[3].GetControlValue() == 69);
        TEST_TRUE(constArray[4].GetControlValue() == 77);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 2);
    TEST_TRUE(Test::Object::GetConstructCount() == 7);
    TEST_TRUE(Test::Object::GetDestructCount() == 7);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array clear
    {
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array;
        array.Resize(8);
        array.Clear();
        TEST_TRUE(memoryStats.ValidateAllocations(1, 8* sizeof(Test::Object)));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(Test::Object));
        TEST_TRUE(array.GetSize() == 0);
        TEST_TRUE(array.GetSizeBytes() == 0 * sizeof(Test::Object));
        TEST_TRUE(array.GetUnusedCapacity() == 8);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 8 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 0);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 8);
        TEST_TRUE(Test::Object::GetDestructCount() == 8);
        TEST_TRUE(Test::Object::GetInstanceCount() == 0);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 8);
    TEST_TRUE(Test::Object::GetDestructCount() == 8);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array shrink to fit
    {
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array;
        array.Reserve(8);
        array.Resize(3, 42);
        array.ShrinkToFit();
        TEST_TRUE(memoryStats.ValidateAllocations(1, 3 * sizeof(Test::Object)));

        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 3);
        TEST_TRUE(array.GetCapacityBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(array.GetSize() == 3);
        TEST_TRUE(array.GetSizeBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(array.GetUnusedCapacity() == 0);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 0 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 0);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 3);
        TEST_TRUE(Test::Object::GetDestructCount() == 0);
        TEST_TRUE(Test::Object::GetInstanceCount() == 3);
        TEST_TRUE(array[0].GetControlValue() == 42);
        TEST_TRUE(array[1].GetControlValue() == 42);
        TEST_TRUE(array[2].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 3);
    TEST_TRUE(Test::Object::GetDestructCount() == 3);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array copy
    {
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array1;
        array1.Reserve(8);
        array1.Resize(3, 42);
        Array<Test::Object> array2 = array1;

        TEST_TRUE(array1.GetData() != nullptr);
        TEST_TRUE(array1.GetCapacity() == 8);
        TEST_TRUE(array1.GetCapacityBytes() == 8 * sizeof(Test::Object));
        TEST_TRUE(array1.GetSize() == 3);
        TEST_TRUE(array1.GetSizeBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(array1.GetUnusedCapacity() == 5);
        TEST_TRUE(array1.GetUnusedCapacityBytes() == 5 * sizeof(Test::Object));
        TEST_TRUE(array2.GetData() != nullptr);
        TEST_TRUE(array2.GetCapacity() == 3);
        TEST_TRUE(array2.GetCapacityBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(array2.GetSize() == 3);
        TEST_TRUE(array2.GetSizeBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(array2.GetUnusedCapacity() == 0);
        TEST_TRUE(array2.GetUnusedCapacityBytes() == 0 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 3);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 6);
        TEST_TRUE(Test::Object::GetDestructCount() == 0);
        TEST_TRUE(Test::Object::GetInstanceCount() == 6);
        TEST_TRUE(array1[0].GetControlValue() == 42);
        TEST_TRUE(array1[1].GetControlValue() == 42);
        TEST_TRUE(array1[2].GetControlValue() == 42);
        TEST_TRUE(array2[0].GetControlValue() == 42);
        TEST_TRUE(array2[1].GetControlValue() == 42);
        TEST_TRUE(array2[2].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 3);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 6);
    TEST_TRUE(Test::Object::GetDestructCount() == 6);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array move
    {
        Test::Object::ResetGlobalCounters();

        Array<Test::Object> array1;
        array1.Reserve(8);
        array1.Resize(3, 42);

        Array<Test::Object> array2 = std::move(array1);
        TEST_TRUE(memoryStats.ValidateAllocations(1, 8 * sizeof(Test::Object)));

        TEST_TRUE(array1.GetData() == nullptr);
        TEST_TRUE(array1.GetCapacity() == 0);
        TEST_TRUE(array1.GetCapacityBytes() == 0);
        TEST_TRUE(array1.GetSize() == 0);
        TEST_TRUE(array1.GetSizeBytes() == 0);
        TEST_TRUE(array1.GetUnusedCapacity() == 0);
        TEST_TRUE(array1.GetUnusedCapacityBytes() == 0);
        TEST_TRUE(array2.GetData() != nullptr);
        TEST_TRUE(array2.GetCapacity() == 8);
        TEST_TRUE(array2.GetCapacityBytes() == 8 * sizeof(Test::Object));
        TEST_TRUE(array2.GetSize() == 3);
        TEST_TRUE(array2.GetSizeBytes() == 3 * sizeof(Test::Object));
        TEST_TRUE(array2.GetUnusedCapacity() == 5);
        TEST_TRUE(array2.GetUnusedCapacityBytes() == 5 * sizeof(Test::Object));
        TEST_TRUE(Test::Object::GetCopyCount() == 0);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 3);
        TEST_TRUE(Test::Object::GetDestructCount() == 0);
        TEST_TRUE(Test::Object::GetInstanceCount() == 3);
        TEST_TRUE(array2[0].GetControlValue() == 42);
        TEST_TRUE(array2[1].GetControlValue() == 42);
        TEST_TRUE(array2[2].GetControlValue() == 42);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 3);
    TEST_TRUE(Test::Object::GetDestructCount() == 3);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array initializer list
    Test::Object::ResetGlobalCounters();

    {
        Array<Test::Object> array = { 42, 42, 42 };
        TEST_TRUE(array.GetSize() == 3);
        TEST_TRUE(array[0].GetControlValue() == 42);
        TEST_TRUE(array[1].GetControlValue() == 42);
        TEST_TRUE(array[2].GetControlValue() == 42);
    }

    TEST_TRUE(Test::Object::GetCopyCount() == 3);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 6);
    TEST_TRUE(Test::Object::GetDestructCount() == 6);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test array contains/find element
    {
        Array<Test::Object> array = { 7, 13, 42, 69 };

        TEST_TRUE(array.Contains(7));
        TEST_TRUE(array.Contains(13));
        TEST_TRUE(array.Contains(42));
        TEST_TRUE(array.Contains(69));
        TEST_FALSE(array.Contains(128));

        TEST_TRUE(array.Find(7) == &array[0]);
        TEST_TRUE(array.Find(13) == &array[1]);
        TEST_TRUE(array.Find(42) == &array[2]);
        TEST_TRUE(array.Find(69) == &array[3]);
        TEST_TRUE(array.Find(128) == nullptr);

        auto predicateGood = [](const Test::Object& object)
        {
            return object == 69;
        };

        auto predicateBad = [](const Test::Object& object)
        {
            return object == 128;
        };

        TEST_TRUE(array.ContainsPredicate(predicateGood));
        TEST_FALSE(array.ContainsPredicate(predicateBad));

        TEST_TRUE(array.FindPredicate(predicateGood) == &array[3]);
        TEST_TRUE(array.FindPredicate(predicateBad) == nullptr);
    }

    return Test::Result::Success;
}
