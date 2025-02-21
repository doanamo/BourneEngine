#pragma once

namespace Test
{
    class Object
    {
        u64 m_controlValue = 0;

        static std::atomic<u64> s_copyCount;
        static std::atomic<u64> s_moveCount;
        static std::atomic<u64> s_constructCount;
        static std::atomic<u64> s_destructCount;
        static std::atomic<u64> s_instanceCount;

    public:
        Object(const u64 controlValue = 0)
            : m_controlValue(controlValue)
        {
            ++s_constructCount;
            ++s_instanceCount;
        }

        virtual ~Object()
        {
            ++s_destructCount;
            --s_instanceCount;
        }
        
        Object(const Object& other)
        {
            ++s_constructCount;
            ++s_instanceCount;
            *this = other;
        }

        Object& operator=(const Object& other)
        {
            ++s_copyCount;
            m_controlValue = other.m_controlValue;
            return *this;
        }

        Object(Object&& other) noexcept
        {
            ++s_constructCount;
            ++s_instanceCount;
            *this = std::move(other);
        }

        Object& operator=(Object&& other) noexcept
        {
            ++s_moveCount;
            m_controlValue = other.m_controlValue;
            other.m_controlValue = 0;
            return *this;
        }

        u64 GetControlValue() const
        {
            return m_controlValue;
        }

        void SetControlValue(const u64 controlValue)
        {
            m_controlValue = controlValue;
        }

        virtual bool IsDerived() const
        {
            return false;
        }

        bool operator==(const Object& other) const
        {
            return m_controlValue == other.m_controlValue;
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

    class ObjectDerived final : public Object
    {
    public:
        explicit ObjectDerived(const u64 controlValue = 0)
            : Object(controlValue)
        {}

        bool IsDerived() const override
        {
            return true;
        }
    };
}
