#include "Shared.hpp"

TEST_DEFINE("Common.Array", "Empty")
{
    Array<u32> array;
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(0, 0));

    TEST_TRUE(array.GetData() == nullptr);
    TEST_TRUE(array.GetCapacity() == 0);
    TEST_TRUE(array.GetCapacityBytes() == 0);
    TEST_TRUE(array.GetSize() == 0);
    TEST_TRUE(array.GetSizeBytes() == 0);
    TEST_TRUE(array.GetUnusedCapacity() == 0);
    TEST_TRUE(array.GetUnusedCapacityBytes() == 0);

    const Array<u32>& constArray = array;
    TEST_TRUE(constArray.GetData() == nullptr);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
}

TEST_DEFINE("Common.Array", "Reserve")
{
    Array<Test::Object> array;
    array.Reserve(5);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 5));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    const void* allocatedData = array.GetData();
    TEST_TRUE(allocatedData != nullptr);

    TEST_TRUE(array.GetData() == allocatedData);
    TEST_TRUE(array.GetCapacity() == 5);
    TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(Test::Object));
    TEST_TRUE(array.GetSize() == 0);
    TEST_TRUE(array.GetSizeBytes() == 0);
    TEST_TRUE(array.GetUnusedCapacity() == 5);
    TEST_TRUE(array.GetUnusedCapacityBytes() == 5 * sizeof(Test::Object));

    const Array<Test::Object>& constArray = array;
    TEST_TRUE(constArray.GetData() == allocatedData);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 5));
    TEST_TRUE(objectGuard.ValidateTotalCounts(0, 0, 0, 0));
}

TEST_DEFINE("Common.Array", "Resize")
{
    Array<Test::Object> array;
    array.Resize(1, 69); // Construct 1 initial object.
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    array.Resize(2, 18); // Construct 1 additional object.
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 2));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(2));

    array.Resize(5, Test::Object(42)); // Fill 3 new objects via copy construction.
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 5));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(5));

    array.Resize(4);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 5));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(4));

    TEST_TRUE(array.GetData() != nullptr);
    TEST_TRUE(array.GetCapacity() == 5);
    TEST_TRUE(array.GetCapacityBytes() == 5 * sizeof(Test::Object));
    TEST_TRUE(array.GetSize() == 4);
    TEST_TRUE(array.GetSizeBytes() == 4 * sizeof(Test::Object));
    TEST_TRUE(array.GetUnusedCapacity() == 1);
    TEST_TRUE(array.GetUnusedCapacityBytes() == 1 * sizeof(Test::Object));
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

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 5));
    TEST_TRUE(objectGuard.ValidateTotalCounts(6, 2, 3, 0));
}

TEST_DEFINE("Common.Array", "Add")
{
    Array<u32> array;
    array.Add();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 4));

    array.Add(18);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 4));

    array.Add(42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 4));

    array.Add(69);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 4));

    array.Add(77);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 8));

    array.Resize(32, 23);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 32));

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

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(u32) * 32));
}

TEST_DEFINE("Common.Array", "AddObject")
{
    Array<Test::Object> array;
    array.Add();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    array.Add(18);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(2));

    array.Add(Test::Object(42));
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(3));

    array.Add(69);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(4));

    array.Add(Test::Object(77));
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(5));

    TEST_TRUE(array.GetData() != nullptr);
    TEST_TRUE(array.GetCapacity() == 8);
    TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(Test::Object));
    TEST_TRUE(array.GetSize() == 5);
    TEST_TRUE(array.GetSizeBytes() == 5 * sizeof(Test::Object));
    TEST_TRUE(array.GetUnusedCapacity() == 3);
    TEST_TRUE(array.GetUnusedCapacityBytes() == 3 * sizeof(Test::Object));
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

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateTotalCounts(7, 2, 0, 2));
}

TEST_DEFINE("Common.Array", "Clear")
{
    Array<Test::Object> array;
    array.Resize(8);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(8));

    array.Clear();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    TEST_TRUE(array.GetData() != nullptr);
    TEST_TRUE(array.GetCapacity() == 8);
    TEST_TRUE(array.GetCapacityBytes() == 8 * sizeof(Test::Object));
    TEST_TRUE(array.GetSize() == 0);
    TEST_TRUE(array.GetSizeBytes() == 0 * sizeof(Test::Object));
    TEST_TRUE(array.GetUnusedCapacity() == 8);
    TEST_TRUE(array.GetUnusedCapacityBytes() == 8 * sizeof(Test::Object));

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateTotalCounts(8, 8, 0, 0));
}

TEST_DEFINE("Common.Array", "ShrinkToFit")
{
    Array<Test::Object> array;
    array.Reserve(8);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    array.Resize(3, 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(3));

    array.ShrinkToFit();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 3));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(3));

    TEST_TRUE(array.GetData() != nullptr);
    TEST_TRUE(array.GetCapacity() == 3);
    TEST_TRUE(array.GetCapacityBytes() == 3 * sizeof(Test::Object));
    TEST_TRUE(array.GetSize() == 3);
    TEST_TRUE(array.GetSizeBytes() == 3 * sizeof(Test::Object));
    TEST_TRUE(array.GetUnusedCapacity() == 0);
    TEST_TRUE(array.GetUnusedCapacityBytes() == 0 * sizeof(Test::Object));
    TEST_TRUE(array[0].GetControlValue() == 42);
    TEST_TRUE(array[1].GetControlValue() == 42);
    TEST_TRUE(array[2].GetControlValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateTotalCounts(3, 0, 0, 0));
}

TEST_DEFINE("Common.Array", "Copy")
{
    Array<Test::Object> array1;
    array1.Reserve(8);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    array1.Resize(3, 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(3));

    Array<Test::Object> array2 = array1;
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(2, sizeof(Test::Object) * 11));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(6));

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
    TEST_TRUE(array1[0].GetControlValue() == 42);
    TEST_TRUE(array1[1].GetControlValue() == 42);
    TEST_TRUE(array1[2].GetControlValue() == 42);
    TEST_TRUE(array2[0].GetControlValue() == 42);
    TEST_TRUE(array2[1].GetControlValue() == 42);
    TEST_TRUE(array2[2].GetControlValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(2, sizeof(Test::Object) * 11));
    TEST_TRUE(objectGuard.ValidateTotalCounts(6, 0, 3, 0));
}

TEST_DEFINE("Common.Array", "Move")
{
    Array<Test::Object> array1;
    array1.Reserve(8);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    array1.Resize(3, 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(3));

    Array<Test::Object> array2 = Move(array1);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(3));

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
    TEST_TRUE(array2[0].GetControlValue() == 42);
    TEST_TRUE(array2[1].GetControlValue() == 42);
    TEST_TRUE(array2[2].GetControlValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 8));
    TEST_TRUE(objectGuard.ValidateTotalCounts(3, 0, 0, 0));
}

TEST_DEFINE("Common.Array", "InitializerList")
{
    // #todo: Fix initializer list copying elements instead of moving them.
    Array<Test::Object> array = { 42, 42, 42 };
    TEST_TRUE(array.GetSize() == 3);
    TEST_TRUE(array[0].GetControlValue() == 42);
    TEST_TRUE(array[1].GetControlValue() == 42);
    TEST_TRUE(array[2].GetControlValue() == 42);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 3));
    TEST_TRUE(objectGuard.ValidateTotalCounts(6, 3, 3, 0));
}

TEST_DEFINE("Common.Array", "Contains")
{
    auto predicateGood = [](const Test::Object& object)
    {
        return object == 69;
    };

    auto predicateBad = [](const Test::Object& object)
    {
        return object == 128;
    };

    Array<Test::Object> array;
    array.Add(7);
    array.Add(13);
    array.Add(42);
    array.Add(69);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(4));

    TEST_TRUE(array.Contains(7));
    TEST_TRUE(array.Contains(13));
    TEST_TRUE(array.Contains(42));
    TEST_TRUE(array.Contains(69));
    TEST_FALSE(array.Contains(128));
    TEST_TRUE(array.ContainsPredicate(predicateGood));
    TEST_FALSE(array.ContainsPredicate(predicateBad));

    const Array<Test::Object>& constArray = array;

    TEST_TRUE(constArray.Contains(7));
    TEST_TRUE(constArray.Contains(13));
    TEST_TRUE(constArray.Contains(42));
    TEST_TRUE(constArray.Contains(69));
    TEST_FALSE(constArray.Contains(128));
    TEST_TRUE(constArray.ContainsPredicate(predicateGood));
    TEST_FALSE(constArray.ContainsPredicate(predicateBad));

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateTotalCounts(30, 26, 0, 0));
}

TEST_DEFINE("Common.Array", "Finds")
{
    auto predicateGood = [](const Test::Object& object)
    {
        return object == 69;
    };

    auto predicateBad = [](const Test::Object& object)
    {
        return object == 128;
    };

    Array<Test::Object> array;
    array.Add(7);
    array.Add(13);
    array.Add(42);
    array.Add(69);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(4));

    TEST_TRUE(array.Find(7) == &array[0]);
    TEST_TRUE(array.Find(13) == &array[1]);
    TEST_TRUE(array.Find(42) == &array[2]);
    TEST_TRUE(array.Find(69) == &array[3]);
    TEST_TRUE(array.Find(128) == nullptr);
    TEST_TRUE(array.FindPredicate(predicateGood) == &array[3]);
    TEST_TRUE(array.FindPredicate(predicateBad) == nullptr);

    const Array<Test::Object>& constArray = array;

    TEST_TRUE(constArray.Find(7) == &constArray[0]);
    TEST_TRUE(constArray.Find(13) == &constArray[1]);
    TEST_TRUE(constArray.Find(42) == &constArray[2]);
    TEST_TRUE(constArray.Find(69) == &constArray[3]);
    TEST_TRUE(constArray.Find(128) == nullptr);
    TEST_TRUE(constArray.FindPredicate(predicateGood) == &constArray[3]);
    TEST_TRUE(constArray.FindPredicate(predicateBad) == nullptr);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateTotalCounts(30, 26, 0, 0));
}
