#pragma once

class TestObject
{
private:
    u64 m_controlValue = 0;

    static u64 g_copyCount;
    static u64 g_moveCount;
    static u64 g_constructCount;
    static u64 g_destructCount;
    static u64 g_instanceCount;

public:
    TestObject(u64 controlValue = 0)
        : m_controlValue(controlValue)
    {
        g_constructCount++;
        g_instanceCount++;
    }

    virtual ~TestObject()
    {
        g_destructCount++;
        g_instanceCount--;
    }

    TestObject(const TestObject& other)
    {
        g_constructCount++;
        g_instanceCount++;
        *this = other;
    }

    TestObject& operator=(const TestObject& other)
    {
        g_copyCount++;
        m_controlValue = other.m_controlValue;
        return *this;
    }

    TestObject(TestObject&& other) noexcept
    {
        g_constructCount++;
        g_instanceCount++;
        *this = std::move(other);
    }

    TestObject& operator=(TestObject&& other) noexcept
    {
        g_moveCount++;
        std::swap(m_controlValue, other.m_controlValue);
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
        g_copyCount = 0;
        g_moveCount = 0;
        g_constructCount = 0;
        g_destructCount = 0;
        g_instanceCount = 0;
    }

    static u64 GetGlobalCopyCount()
    {
        return g_copyCount;
    }

    static u64 GetGlobalMoveCount()
    {
        return g_moveCount;
    }

    static u64 GetGlobalConstructCount()
    {
        return g_constructCount;
    }

    static u64 GetGlobalDestructCount()
    {
        return g_destructCount;
    }

    static u64 GetGlobalInstanceCount()
    {
        return g_instanceCount;
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
