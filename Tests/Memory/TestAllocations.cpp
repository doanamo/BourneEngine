#include "Shared.hpp"

TEST_DEFINE("Memory.Allocations", "Basic")
{
    u32* value = Memory::Allocate<u32>();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32)));
    TEST_TRUE(value != nullptr);

    *value = 42;
    TEST_TRUE(*value == 42);

    Memory::Deallocate(value, 1);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(u32)));
}

TEST_DEFINE("Memory.Allocations", "Aligned")
{
    struct alignas(64) TestStruct
    {
        explicit TestStruct(const u32 value)
            : value(value)
        {
        }

        u32 value;
        u8 padding[60] = {};
    };

    TestStruct* value = Memory::New<TestStruct>(42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(TestStruct)));
    TEST_TRUE(value != nullptr);
    TEST_TRUE(value->value == 42);

    value->value = 17;
    TEST_TRUE(value->value == 17);

    Memory::Delete(value);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(TestStruct)));
}

TEST_DEFINE("Memory.Allocations", "Array")
{
    u32* values = Memory::Allocate<u32>(4);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 4));
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
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(u32) * 4));
 }

TEST_DEFINE("Memory.Allocations", "Reallocate")
{
    auto ValidateAssign = [](u32* values, u64 previousSize, u64 newSize) -> bool
    {
        for(u32 i = 0; i < std::min(previousSize, newSize); i++)
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
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32)));
    TEST_TRUE(values != nullptr);

    *values = 1;
    TEST_TRUE(*values == 1);

    values = Memory::Reallocate(values, 8, 1);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 8));
    TEST_TRUE(values != nullptr);
    TEST_TRUE(ValidateAssign(values, 1, 8));

    values = Memory::Reallocate(values, 64, 8);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 64));
    TEST_TRUE(values != nullptr);
    TEST_TRUE(ValidateAssign(values, 8, 64));

    values = Memory::Reallocate(values, 1024, 64);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 1024));
    TEST_TRUE(values != nullptr);
    TEST_TRUE(ValidateAssign(values, 64, 1024));

    values = Memory::Reallocate(values, 4, 1024);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u32) * 4));
    TEST_TRUE(values != nullptr);
    TEST_TRUE(ValidateAssign(values, 1024, 4));

    Memory::Deallocate(values, 4);
}

TEST_DEFINE("Memory.Allocations", "TrivialConstruction")
{
    u64* trivial = Memory::Allocate<u64>();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u64)));
    TEST_TRUE(trivial != nullptr);

    Memory::Construct(trivial, 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u64)));
    TEST_TRUE(*trivial == 42);

    Memory::Destruct(trivial);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u64)));

    Memory::Deallocate(trivial, 1);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(u64)));
}

TEST_DEFINE("Memory.Allocations", "ObjectConstruction")
{
    Test::Object* object = Memory::Allocate<Test::Object>();
    TEST_TRUE(object != nullptr);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    Memory::Construct(object, 42);
    TEST_TRUE(object->GetControlValue() == 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    Memory::Destruct(object);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    Memory::Deallocate(object, 1);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 1, 0, 0));
}

TEST_DEFINE("Memory.Allocations", "TrivialArrayConstruction")
{
    u64* objects = Memory::Allocate<u64>(4);
    TEST_TRUE(objects != nullptr);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u64) * 4));

    Memory::ConstructRange(objects, objects + 4, 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u64) * 4));

    for(u32 i = 0; i < 4; i++)
    {
        TEST_TRUE(objects[i] == 42);
    }

    Memory::DestructRange(objects, objects + 4);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(u64) * 4));

    Memory::Deallocate(objects, 4);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(u64) * 4));
}

TEST_DEFINE("Memory.Allocations", "ObjectArrayConstruction")
{
    Test::Object* objects = Memory::Allocate<Test::Object>(4);
    TEST_TRUE(objects != nullptr);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    Memory::ConstructRange(objects, objects + 4, 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(4));

    for(u32 i = 0; i < 4; i++)
    {
        TEST_TRUE(objects[i].GetControlValue() == 42);
    }

    Memory::DestructRange(objects, objects + 4);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    Memory::Deallocate(objects, 4);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object) * 4));
    TEST_TRUE(objectGuard.ValidateTotalCounts(4, 4, 0, 0));
}

TEST_DEFINE("Memory.Allocations", "New")
{
    Test::Object* object = Memory::New<Test::Object>(42);
    TEST_TRUE(object != nullptr);
    TEST_TRUE(object->GetControlValue() == 42);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    Memory::Delete(object);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 1, 0, 0));
}

TEST_DEFINE("Memory.Allocations", "AlignSize")
{
    TEST_TRUE(Memory::AlignSize(0, 4) == 0);
    TEST_TRUE(Memory::AlignSize(1, 4) == 4);
    TEST_TRUE(Memory::AlignSize(2, 4) == 4);
    TEST_TRUE(Memory::AlignSize(3, 4) == 4);
    TEST_TRUE(Memory::AlignSize(4, 4) == 4);
    TEST_TRUE(Memory::AlignSize(5, 4) == 8);
    TEST_TRUE(Memory::AlignSize(6, 4) == 8);
    TEST_TRUE(Memory::AlignSize(7, 4) == 8);
    TEST_TRUE(Memory::AlignSize(8, 4) == 8);
}
