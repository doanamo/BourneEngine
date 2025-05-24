#pragma once

namespace Test
{
    class Object
    {
        u64 m_controlValue = 0;

        static std::atomic<u64> s_copyTotalCount;
        static std::atomic<u64> s_moveTotalCount;
        static std::atomic<u64> s_constructTotalCount;
        static std::atomic<u64> s_destructTotalCount;
        static std::atomic<u64> s_instanceTotalCount; // #todo: Remove because it always equals construct count
        static std::atomic<u64> s_instanceCurrentCount;

    public:
        Object(const u64 controlValue = 0)
            : m_controlValue(controlValue)
        {
            ++s_constructTotalCount;
            ++s_instanceTotalCount;
            ++s_instanceCurrentCount;
        }

        virtual ~Object()
        {
            ++s_destructTotalCount;
            --s_instanceCurrentCount;
        }
        
        Object(const Object& other)
        {
            ++s_constructTotalCount;
            ++s_instanceTotalCount;
            ++s_instanceCurrentCount;
            *this = other;
        }

        Object& operator=(const Object& other)
        {
            ++s_copyTotalCount;
            m_controlValue = other.m_controlValue;
            return *this;
        }

        Object(Object&& other) noexcept
        {
            ++s_constructTotalCount;
            ++s_instanceTotalCount;
            ++s_instanceCurrentCount;
            *this = Move(other);
        }

        Object& operator=(Object&& other) noexcept
        {
            ++s_moveTotalCount;
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

        static u64 GetCopyTotalCount()
        {
            return s_copyTotalCount;
        }

        static u64 GetMoveTotalCount()
        {
            return s_moveTotalCount;
        }

        static u64 GetConstructTotalCount()
        {
            return s_constructTotalCount;
        }

        static u64 GetDestructTotalCount()
        {
            return s_destructTotalCount;
        }

        static u64 GetInstanceTotalCount()
        {
            return s_instanceTotalCount;
        }

        static u64 GetInstanceCurrentCount()
        {
            return s_instanceCurrentCount;
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
