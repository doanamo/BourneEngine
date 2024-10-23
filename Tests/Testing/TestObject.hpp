#pragma once

class TestObject
{
private:
    u64 m_controlValue = 0;

    static std::atomic<u64> s_copyCount;
    static std::atomic<u64> s_moveCount;
    static std::atomic<u64> s_constructCount;
    static std::atomic<u64> s_destructCount;
    static std::atomic<u64> s_instanceCount;

public:
    TestObject(u64 controlValue = 0)
        : m_controlValue(controlValue)
    {
        s_constructCount++;
        s_instanceCount++;
    }

    virtual ~TestObject()
    {
        s_destructCount++;
        s_instanceCount--;
    }

    TestObject(const TestObject& other)
    {
        s_constructCount++;
        s_instanceCount++;
        *this = other;
    }

    TestObject& operator=(const TestObject& other)
    {
        s_copyCount++;
        m_controlValue = other.m_controlValue;
        return *this;
    }

    TestObject(TestObject&& other) noexcept
    {
        s_constructCount++;
        s_instanceCount++;
        *this = std::move(other);
    }

    TestObject& operator=(TestObject&& other) noexcept
    {
        s_moveCount++;
        m_controlValue = other.m_controlValue;
        other.m_controlValue = 0;
        return *this;
    }

    u64 GetControlValue() const
    {
        return m_controlValue;
    }

    void SetControlValue(u64 controlValue)
    {
        m_controlValue = controlValue;
    }

    virtual bool IsDerived() const
    {
        return false;
    }

    static void ResetGlobalCounters()
    {
        s_copyCount = 0;
        s_moveCount = 0;
        s_constructCount = 0;
        s_destructCount = 0;
        s_instanceCount = 0;
    }

    static u64 GetCopyCount()
    {
        return s_copyCount;
    }

    static u64 GetMoveCount()
    {
        return s_moveCount;
    }

    static u64 GetConstructCount()
    {
        return s_constructCount;
    }

    static u64 GetDestructCount()
    {
        return s_destructCount;
    }

    static u64 GetInstanceCount()
    {
        return s_instanceCount;
    }
};

class TestObjectDerived : public TestObject
{
public:
    TestObjectDerived(u64 controlValue = 0)
        : TestObject(controlValue)
    {
    }

    bool IsDerived() const override
    {
        return true;
    }
};
