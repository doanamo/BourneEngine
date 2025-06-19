#include "Shared.hpp"

template<u64 Size>
class TestDeleter
{
    u8 padding[Size] = {};
    void operator()(void* pointer) const {}
};

TEST_DEFINE("Common.UniquePtr", "Empty")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    UniquePtr<Test::Object> ptr;
    TEST_FALSE(ptr);
    TEST_TRUE(ptr == ptr);
    TEST_FALSE(ptr != ptr);
    TEST_TRUE(ptr == nullptr);
    TEST_FALSE(ptr != nullptr);
    TEST_TRUE(ptr.Get() == nullptr);
    TEST_FALSE(ptr.Get() != nullptr);

    const UniquePtr<Test::Object>& constPtr = ptr;
    TEST_FALSE(constPtr);
    TEST_TRUE(constPtr == constPtr);
    TEST_FALSE(constPtr != constPtr);
    TEST_TRUE(constPtr == nullptr);
    TEST_FALSE(constPtr != nullptr);
    TEST_TRUE(constPtr.Get() == nullptr);
    TEST_FALSE(constPtr.Get() != nullptr);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(0, 0));
    TEST_TRUE(objectGuard.ValidateTotalCounts(0, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "AssignPointer")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    UniquePtr ptr = Memory::New<Test::Object>(64);
    Test::Object& ref = *ptr;
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    TEST_TRUE(ptr);
    TEST_TRUE(ptr == ptr);
    TEST_FALSE(ptr != ptr);
    TEST_TRUE(ptr != nullptr);
    TEST_FALSE(ptr == nullptr);
    TEST_TRUE(ptr.Get() != nullptr);
    TEST_FALSE(ptr.Get() == nullptr);
    TEST_TRUE(ptr->GetControlValue() == 64);
    TEST_TRUE(ref.GetControlValue() == 64);

    const UniquePtr<Test::Object>& constPtr = ptr;
    const Test::Object& constRef = *constPtr;

    TEST_TRUE(constPtr);
    TEST_TRUE(constPtr == constPtr);
    TEST_FALSE(constPtr != constPtr);
    TEST_TRUE(constPtr != nullptr);
    TEST_FALSE(constPtr == nullptr);
    TEST_TRUE(constPtr.Get() != nullptr);
    TEST_FALSE(constPtr.Get() == nullptr);
    TEST_TRUE(constPtr->GetControlValue() == 64);
    TEST_TRUE(constRef.GetControlValue() == 64);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "MoveOperator")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    UniquePtr ptr = Memory::New<Test::Object>(64);
    UniquePtr<Test::Object> ptrMoved = Move(ptr);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    TEST_FALSE(ptr);
    TEST_TRUE(ptr == ptr);
    TEST_FALSE(ptr != ptr);
    TEST_TRUE(ptr == nullptr);
    TEST_FALSE(ptr != nullptr);
    TEST_TRUE(ptr.Get() == nullptr);
    TEST_FALSE(ptr.Get() != nullptr);

    Test::Object& refMoved = *ptrMoved;

    TEST_TRUE(ptrMoved);
    TEST_TRUE(ptrMoved == ptrMoved);
    TEST_FALSE(ptrMoved != ptrMoved);
    TEST_TRUE(ptrMoved != nullptr);
    TEST_FALSE(ptrMoved == nullptr);
    TEST_TRUE(ptrMoved.Get() != nullptr);
    TEST_FALSE(ptrMoved.Get() == nullptr);
    TEST_TRUE(ptrMoved->GetControlValue() == 64);
    TEST_TRUE(refMoved.GetControlValue() == 64);

    const UniquePtr<Test::Object>& constPtr = ptrMoved;
    const Test::Object& constRefMoved = *constPtr;

    TEST_TRUE(constPtr);
    TEST_TRUE(constPtr == constPtr);
    TEST_FALSE(constPtr != constPtr);
    TEST_TRUE(constPtr != nullptr);
    TEST_FALSE(constPtr == nullptr);
    TEST_TRUE(constPtr.Get() != nullptr);
    TEST_FALSE(constPtr.Get() == nullptr);
    TEST_TRUE(constPtr->GetControlValue() == 64);
    TEST_TRUE(constRefMoved.GetControlValue() == 64);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "Reset")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    UniquePtr ptr = Memory::New<Test::Object>(64);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    ptr.Reset();
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(0, 0));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(0));

    TEST_FALSE(ptr);
    TEST_TRUE(ptr == ptr);
    TEST_FALSE(ptr != ptr);
    TEST_TRUE(ptr == nullptr);
    TEST_FALSE(ptr != nullptr);
    TEST_TRUE(ptr.Get() == nullptr);
    TEST_FALSE(ptr.Get() != nullptr);

    const UniquePtr<Test::Object>& constPtr = ptr;
    TEST_FALSE(constPtr);
    TEST_TRUE(constPtr == constPtr);
    TEST_FALSE(constPtr != constPtr);
    TEST_TRUE(constPtr == nullptr);
    TEST_FALSE(constPtr != nullptr);
    TEST_TRUE(constPtr.Get() == nullptr);
    TEST_FALSE(constPtr.Get() != nullptr);

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 1, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "Detach")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    Test::Object* detached = nullptr;
    {
        UniquePtr ptr = Memory::New<Test::Object>(64);
        TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
        TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

        detached = ptr.Detach();
        TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));
        TEST_TRUE(objectGuard.ValidateCurrentInstances(1));
    }

    TEST_TRUE(detached != nullptr);
    TEST_TRUE(detached->GetControlValue() == 64);

    Memory::Delete<Test::Object>(detached);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 1, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "Inheritance")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    UniquePtr ptrDerived = Memory::New<Test::ObjectDerived>(64);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::ObjectDerived)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    UniquePtr<Test::Object> ptrBase = Move(ptrDerived);
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::ObjectDerived)));
    TEST_TRUE(objectGuard.ValidateCurrentInstances(1));

    TEST_FALSE(ptrDerived);
    TEST_TRUE(ptrBase);
    TEST_TRUE(ptrBase->GetControlValue() == 64);
    TEST_TRUE(ptrBase->IsDerived());

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "SizedDeleter")
{
    static_assert(sizeof(UniquePtr<u8, TestDeleter<1>>) == 16);
    static_assert(sizeof(UniquePtr<u8, TestDeleter<4>>) == 16);
    static_assert(sizeof(UniquePtr<u8, TestDeleter<8>>) == 16);
    static_assert(sizeof(UniquePtr<u8, TestDeleter<16>>) == 24);
    static_assert(sizeof(UniquePtr<u8, TestDeleter<32>>) == 40);

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "LambdaDeleter")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    auto objectDeleter = [](Test::ObjectDerived* pointer)
    {
        Memory::Delete(pointer);
    };

    UniquePtr<Test::ObjectDerived, decltype(objectDeleter)> ptr(Memory::New<Test::ObjectDerived>());
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "VoidLambdaDeleter")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    auto voidDeleter = [](void* pointer)
    {
        Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
    };

    UniquePtr<Test::ObjectDerived, decltype(voidDeleter)> ptr(Memory::New<Test::ObjectDerived>());
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "ErasedVoidLambdaDeleter")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    auto* obj1 = Memory::New<Test::ObjectDerived>();
    UniquePtr<void> ptr = UniquePtr<void>(obj1, [](void* pointer)
    {
        Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
    });
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));

    auto* obj2 = Memory::New<Test::ObjectDerived>();
    ptr.Reset(obj2, [](void* pointer)
    {
        Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
    });
    TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::Object)));

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(2, sizeof(Test::Object) * 2));
    TEST_TRUE(objectGuard.ValidateTotalCounts(2, 1, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "VoidType")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    UniquePtr<void> ptr(Memory::New<Test::ObjectDerived>(),
        [](void* pointer)
        {
            Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
        });

    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 0, 0, 0));

    return Test::Result::Success;
}

TEST_DEFINE("Common.UniquePtr", "CapturingLambdaDeleter")
{
    Test::MemoryGuard memoryGuard;
    Test::ObjectGuard objectGuard;

    bool deleted = false;
    auto captureDeleter = [&deleted](void* pointer)
    {
        Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
        deleted = true;
    };

    {
        UniquePtr<void, decltype(&captureDeleter)> ptr(Memory::New<Test::ObjectDerived>(), &captureDeleter);
        TEST_TRUE(memoryGuard.ValidateCurrentAllocations(1, sizeof(Test::ObjectDerived)));
    }

    TEST_TRUE(deleted);
    TEST_TRUE(memoryGuard.ValidateTotalAllocations(1, sizeof(Test::Object)));
    TEST_TRUE(objectGuard.ValidateTotalCounts(1, 1, 0, 0));

    return Test::Result::Success;
}
