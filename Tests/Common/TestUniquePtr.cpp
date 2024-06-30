#include "Shared.hpp"
#include "TestCommon.hpp"

TestResult Common::TestUniquePtr()
{
    // Test unique empty
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObject> ptr;
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

    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 0);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 0);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    // Test unique make
    {
        TestObject::ResetGlobalCounters();

        UniquePtr<TestObject> ptr = MakeUnique<TestObject>(64);
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

    TEST_TRUE(TestObject::GetGlobalCopyCount() == 0);
    TEST_TRUE(TestObject::GetGlobalMoveCount() == 0);
    TEST_TRUE(TestObject::GetGlobalConstructCount() == 1);
    TEST_TRUE(TestObject::GetGlobalDestructCount() == 1);
    TEST_TRUE(TestObject::GetGlobalInstanceCount() == 0);

    return TestResult::Success;
}
