#include "Shared.hpp"
#include "TestCommon.hpp"
#include "Memory/TestMemoryStats.hpp"

template<u64 Size>
class TestDeleter
{
    u8 padding[Size];

    void operator()(void* pointer)
    {
    }
};

TestResult Common::TestUniquePtr()
{
    LOG_INFO("Running Common::TestUniquePtr...");
    Memory::TestStats memoryStats;

    // Test unique pointer empty
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObject> ptr;
        TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
        TEST_FALSE(ptr);
        TEST_TRUE(ptr == ptr);
        TEST_FALSE(ptr != ptr);
        TEST_TRUE(ptr == nullptr);
        TEST_FALSE(ptr != nullptr);
        TEST_TRUE(ptr.Get() == nullptr);
        TEST_FALSE(ptr.Get() != nullptr);

        const UniquePtr<TestObject>& constPtr = ptr;
        TEST_FALSE(constPtr);
        TEST_TRUE(constPtr == constPtr);
        TEST_FALSE(constPtr != constPtr);
        TEST_TRUE(constPtr == nullptr);
        TEST_FALSE(constPtr != nullptr);
        TEST_TRUE(constPtr.Get() == nullptr);
        TEST_FALSE(constPtr.Get() != nullptr);
    }

    TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 0);
    TEST_TRUE(TestObject::GetConstructCount() == 0);
    TEST_TRUE(TestObject::GetDestructCount() == 0);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    // Test unique pointer make
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObject> ptr = AllocateUnique<TestObject>(64);
        TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObject)));
        TEST_TRUE(ptr);
        TEST_TRUE(ptr == ptr);
        TEST_FALSE(ptr != ptr);
        TEST_TRUE(ptr != nullptr);
        TEST_FALSE(ptr == nullptr);
        TEST_TRUE(ptr.Get() != nullptr);
        TEST_FALSE(ptr.Get() == nullptr);
        TEST_TRUE(ptr->GetControlValue() == 64);
        TEST_TRUE((*ptr).GetControlValue() == 64);

        const UniquePtr<TestObject>& constPtr = ptr;
        TEST_TRUE(constPtr);
        TEST_TRUE(constPtr == constPtr);
        TEST_FALSE(constPtr != constPtr);
        TEST_TRUE(constPtr != nullptr);
        TEST_FALSE(constPtr == nullptr);
        TEST_TRUE(constPtr.Get() != nullptr);
        TEST_FALSE(constPtr.Get() == nullptr);
        TEST_TRUE(constPtr->GetControlValue() == 64);
        TEST_TRUE((*constPtr).GetControlValue() == 64);
    }

    TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 0);
    TEST_TRUE(TestObject::GetConstructCount() == 1);
    TEST_TRUE(TestObject::GetDestructCount() == 1);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    // Test unique pointer move
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObject> ptr = AllocateUnique<TestObject>(64);
        UniquePtr<TestObject> ptrMoved = std::move(ptr);
        TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObject)));

        TEST_FALSE(ptr);
        TEST_TRUE(ptr == ptr);
        TEST_FALSE(ptr != ptr);
        TEST_TRUE(ptr == nullptr);
        TEST_FALSE(ptr != nullptr);
        TEST_TRUE(ptr.Get() == nullptr);
        TEST_FALSE(ptr.Get() != nullptr);

        TEST_TRUE(ptrMoved);
        TEST_TRUE(ptrMoved == ptrMoved);
        TEST_FALSE(ptrMoved != ptrMoved);
        TEST_TRUE(ptrMoved != nullptr);
        TEST_FALSE(ptrMoved == nullptr);
        TEST_TRUE(ptrMoved.Get() != nullptr);
        TEST_FALSE(ptrMoved.Get() == nullptr);
        TEST_TRUE(ptrMoved->GetControlValue() == 64);
        TEST_TRUE((*ptrMoved).GetControlValue() == 64);

        const UniquePtr<TestObject>& constPtr = ptrMoved;
        TEST_TRUE(constPtr);
        TEST_TRUE(constPtr == constPtr);
        TEST_FALSE(constPtr != constPtr);
        TEST_TRUE(constPtr != nullptr);
        TEST_FALSE(constPtr == nullptr);
        TEST_TRUE(constPtr.Get() != nullptr);
        TEST_FALSE(constPtr.Get() == nullptr);
        TEST_TRUE(constPtr->GetControlValue() == 64);
        TEST_TRUE((*constPtr).GetControlValue() == 64);
    }

    TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 0);
    TEST_TRUE(TestObject::GetConstructCount() == 1);
    TEST_TRUE(TestObject::GetDestructCount() == 1);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    // Test unique pointer reset
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObject> ptr = AllocateUnique<TestObject>(64);
        TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObject)));

        ptr.Reset();
        TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));

        TEST_FALSE(ptr);
        TEST_TRUE(ptr == ptr);
        TEST_FALSE(ptr != ptr);
        TEST_TRUE(ptr == nullptr);
        TEST_FALSE(ptr != nullptr);
        TEST_TRUE(ptr.Get() == nullptr);
        TEST_FALSE(ptr.Get() != nullptr);

        const UniquePtr<TestObject>& constPtr = ptr;
        TEST_FALSE(constPtr);
        TEST_TRUE(constPtr == constPtr);
        TEST_FALSE(constPtr != constPtr);
        TEST_TRUE(constPtr == nullptr);
        TEST_FALSE(constPtr != nullptr);
        TEST_TRUE(constPtr.Get() == nullptr);
        TEST_FALSE(constPtr.Get() != nullptr);
    }

    TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 0);
    TEST_TRUE(TestObject::GetConstructCount() == 1);
    TEST_TRUE(TestObject::GetDestructCount() == 1);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    // Test unique pointer release
    {
        TestObject::ResetGlobalCounters();

        TestObject* released = nullptr;
        {
            UniquePtr<TestObject> ptr = AllocateUnique<TestObject>(64);
            TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObject)));

            released = ptr.Detach();
            TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObject)));
        }

        TEST_TRUE(TestObject::GetCopyCount() == 0);
        TEST_TRUE(TestObject::GetMoveCount() == 0);
        TEST_TRUE(TestObject::GetConstructCount() == 1);
        TEST_TRUE(TestObject::GetDestructCount() == 0);
        TEST_TRUE(TestObject::GetInstanceCount() == 1);

        TEST_TRUE(released != nullptr);
        TEST_TRUE(released->GetControlValue() == 64);

        Memory::Delete<TestObject>(released);
        TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    }

    TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 0);
    TEST_TRUE(TestObject::GetConstructCount() == 1);
    TEST_TRUE(TestObject::GetDestructCount() == 1);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    // Test unique pointer inheritance
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObjectDerived> ptrDerived = AllocateUnique<TestObjectDerived>(64);
        TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObjectDerived)));

        UniquePtr<TestObject> ptrBase = std::move(ptrDerived);
        TEST_TRUE(memoryStats.ValidateSystemAllocations(1, sizeof(TestObjectDerived)));

        TEST_FALSE(ptrDerived);
        TEST_TRUE(ptrBase);
        TEST_TRUE(ptrBase->GetControlValue() == 64);
        TEST_TRUE(ptrBase->IsDerived());
    }

    TEST_TRUE(memoryStats.ValidateSystemAllocations(0, 0));
    TEST_TRUE(TestObject::GetCopyCount() == 0);
    TEST_TRUE(TestObject::GetMoveCount() == 0);
    TEST_TRUE(TestObject::GetConstructCount() == 1);
    TEST_TRUE(TestObject::GetDestructCount() == 1);
    TEST_TRUE(TestObject::GetInstanceCount() == 0);

    // Test unique pointer with sized deleter
    {
        static_assert(sizeof(UniquePtr<u8, TestDeleter<1>>) == 16);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<4>>) == 16);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<8>>) == 16);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<16>>) == 24);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<32>>) == 40);
    }

    return TestResult::Success;
}
