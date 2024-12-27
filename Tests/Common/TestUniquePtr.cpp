#include "Shared.hpp"
#include "TestCommon.hpp"
#include "Memory/TestMemoryStats.hpp"

template<u64 Size>
class TestDeleter
{
    u8 padding[Size] = {};
    void operator()(void* pointer) const
    {
    }
};

Test::Result Common::TestUniquePtr()
{
    LOG_INFO("Running Common::TestUniquePtr...");
    const Memory::TestStats memoryStats;

    // Test unique pointer empty
    Test::Object::ResetGlobalCounters();

    {
        UniquePtr<Test::Object> ptr;
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
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
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 0);
    TEST_TRUE(Test::Object::GetDestructCount() == 0);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer make
    Test::Object::ResetGlobalCounters();

    {
        UniquePtr<Test::Object> ptr = AllocateUnique<Test::Object>(64);
        Test::Object& ref = *ptr;

        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object)));
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
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer move
    Test::Object::ResetGlobalCounters();

    {
        UniquePtr<Test::Object> ptr = AllocateUnique<Test::Object>(64);
        UniquePtr<Test::Object> ptrMoved = std::move(ptr);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object)));

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
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer reset
    Test::Object::ResetGlobalCounters();

    {
        UniquePtr<Test::Object> ptr = AllocateUnique<Test::Object>(64);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object)));

        ptr.Reset();
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));

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
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer release
    Test::Object::ResetGlobalCounters();

    {
        Test::Object* released = nullptr;
        {
            UniquePtr<Test::Object> ptr = AllocateUnique<Test::Object>(64);
            TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object)));

            released = ptr.Detach();
            TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::Object)));
        }

        TEST_TRUE(Test::Object::GetCopyCount() == 0);
        TEST_TRUE(Test::Object::GetMoveCount() == 0);
        TEST_TRUE(Test::Object::GetConstructCount() == 1);
        TEST_TRUE(Test::Object::GetDestructCount() == 0);
        TEST_TRUE(Test::Object::GetInstanceCount() == 1);

        TEST_TRUE(released != nullptr);
        TEST_TRUE(released->GetControlValue() == 64);

        Memory::Delete<Test::Object>(released);
        TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer inheritance
    Test::Object::ResetGlobalCounters();

    {
        UniquePtr<Test::ObjectDerived> ptrDerived = AllocateUnique<Test::ObjectDerived>(64);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::ObjectDerived)));

        UniquePtr<Test::Object> ptrBase = std::move(ptrDerived);
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::ObjectDerived)));

        TEST_FALSE(ptrDerived);
        TEST_TRUE(ptrBase);
        TEST_TRUE(ptrBase->GetControlValue() == 64);
        TEST_TRUE(ptrBase->IsDerived());
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer with sized deleter
    {
        static_assert(sizeof(UniquePtr<u8, TestDeleter<1>>) == 16);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<4>>) == 16);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<8>>) == 16);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<16>>) == 24);
        static_assert(sizeof(UniquePtr<u8, TestDeleter<32>>) == 40);
    }

    // Test unique pointer with lambda deleter
    Test::Object::ResetGlobalCounters();

    {
        auto objectDeleter = [](Test::ObjectDerived* pointer)
        {
            Memory::Delete(pointer);
        };

        UniquePtr<Test::ObjectDerived, decltype(objectDeleter)> ptr(Memory::New<Test::ObjectDerived>());
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::ObjectDerived)));
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer with void lambda deleter
    Test::Object::ResetGlobalCounters();

    {
        auto voidDeleter = [](void* pointer)
        {
            Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
        };

        UniquePtr<Test::ObjectDerived, decltype(voidDeleter)> ptr(Memory::New<Test::ObjectDerived>());
        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::ObjectDerived)));
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer with void type
    Test::Object::ResetGlobalCounters();

    {
        UniquePtr<void> ptr(Memory::New<Test::ObjectDerived>(),
            [](void* pointer)
            {
                Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
            });

        TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::ObjectDerived)));
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    // Test unique pointer with capturing lambda deleter
    Test::Object::ResetGlobalCounters();

    {
        bool deleted = false;
        auto captureDeleter = [&deleted](void* pointer)
        {
            Memory::Delete(static_cast<Test::ObjectDerived*>(pointer));
            deleted = true;
        };

        {
            UniquePtr<void, decltype(&captureDeleter)> ptr(Memory::New<Test::ObjectDerived>(), &captureDeleter);
            TEST_TRUE(memoryStats.ValidateAllocations(1, sizeof(Test::ObjectDerived)));
        }

        TEST_TRUE(deleted);
    }

    TEST_TRUE(memoryStats.ValidateAllocations(0, 0));
    TEST_TRUE(Test::Object::GetCopyCount() == 0);
    TEST_TRUE(Test::Object::GetMoveCount() == 0);
    TEST_TRUE(Test::Object::GetConstructCount() == 1);
    TEST_TRUE(Test::Object::GetDestructCount() == 1);
    TEST_TRUE(Test::Object::GetInstanceCount() == 0);

    return Test::Result::Success;
}
