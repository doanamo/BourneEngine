#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestArray()
{
    // Test empty array
    {
        Array<u32> array;
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
        const Array<TestObject>& constArray = array;

        array.Reserve(5);
        const void* allocatedData = array.GetData();
        TEST_TRUE(allocatedData != nullptr);

        TEST_TRUE(constArray.GetData() == allocatedData);
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
    }

    // Test array resize
    {
        TestObject::ResetGlobalCounters();

        Array<TestObject> array;
        const Array<TestObject>& constArray = array;

        array.Resize(1, 69);
        array.Resize(3, 18);
        array.Resize(5, 42);
        array.Resize(4, 666);

        TEST_TRUE(constArray.GetData() != nullptr);
        TEST_TRUE(array.GetData() != nullptr);
        TEST_TRUE(array.GetCapacity() == 5);
        TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 4);
        TEST_TRUE(array.GetSizeBytes() == 4 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 1);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 1 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 5);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 4);
        TEST_TRUE(array[0].GetControlValue() == 69);
        TEST_TRUE(array[1].GetControlValue() == 18);
        TEST_TRUE(array[2].GetControlValue() == 18);
        TEST_TRUE(array[3].GetControlValue() == 42);
    }

    /*
        array.Resize(2, 69);
        TEST_TRUE(constArray.GetData() == allocatedData);
        TEST_TRUE(array.GetData() == allocatedData);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 2);
        TEST_TRUE(array.GetSizeBytes() == 2 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 6);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 6 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);

        array.Resize(array.GetSize() + 2, 42);
        TEST_TRUE(constArray.GetData() == allocatedData);
        TEST_TRUE(array.GetData() == allocatedData);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 3);
        TEST_TRUE(array.GetSizeBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 5);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 3);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 3);
        TEST_TRUE(array[0].GetControlValue() == 0);
        TEST_TRUE(array[1].GetControlValue() == 42);
        TEST_TRUE(array[2].GetControlValue() == 42);

        array.Add();
        TEST_TRUE(constArray.GetData() == allocatedData);
        TEST_TRUE(array.GetData() == allocatedData);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 4);
        TEST_TRUE(array.GetSizeBytes() == 4 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 4);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 4 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 4);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 4);
        TEST_TRUE(array[0].GetControlValue() == 0);
        TEST_TRUE(array[1].GetControlValue() == 42);
        TEST_TRUE(array[2].GetControlValue() == 42);
        TEST_TRUE(array[3].GetControlValue() == 0);

        array.Add(TestObject(13));
        TEST_TRUE(constArray.GetData() == allocatedData);
        TEST_TRUE(array.GetData() == allocatedData);
        TEST_TRUE(array.GetCapacity() == 8);
        TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(TestObject));
        TEST_TRUE(array.GetSize() == 5);
        TEST_TRUE(array.GetSizeBytes() == 5 * sizeof(TestObject));
        TEST_TRUE(array.GetUnusedCapacity() == 3);
        TEST_TRUE(array.GetUnusedCapacityBytes() == 3 * sizeof(TestObject));
        TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
        TEST_TRUE(TestObject::GetGlobalMoveCount() == 1);
        TEST_TRUE(TestObject::GetGlobalConstructCount() == 6);
        TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
        TEST_TRUE(TestObject::GetGlobalInstanceCount() == 5);
        TEST_TRUE(array[0].GetControlValue() == 0);
        TEST_TRUE(array[1].GetControlValue() == 42);
        TEST_TRUE(array[2].GetControlValue() == 42);
        TEST_TRUE(array[3].GetControlValue() == 0);
        TEST_TRUE(array[4].GetControlValue() == 13);
    }
    */

    return TestResult::Success;
}
