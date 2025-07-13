#pragma once

namespace Test
{
    class Object
    {
        u64 m_controlValue;
        bool m_countStats;

        static std::atomic<u64> s_instanceCurrentCount;
        static std::atomic<u64> s_constructTotalCount;
        static std::atomic<u64> s_destructTotalCount;
        static std::atomic<u64> s_copyTotalCount;
        static std::atomic<u64> s_moveTotalCount;

    public:
        Object(const u64 controlValue = 0, bool countStats = true)
            : m_controlValue(controlValue)
            , m_countStats(countStats)
        {
            if(m_countStats)
            {
                ++s_instanceCurrentCount;
                ++s_constructTotalCount;
            }
        }

        virtual ~Object()
        {
            if(m_countStats)
            {
                --s_instanceCurrentCount;
                ++s_destructTotalCount;
            }
        }
        
        Object(const Object& other)
            : m_countStats(other.m_countStats)
        {
            if(m_countStats)
            {
                ++s_constructTotalCount;
                ++s_instanceCurrentCount;
            }

            *this = other;
        }

        Object& operator=(const Object& other)
        {
            if(m_countStats)
            {
                ++s_copyTotalCount;
            }

            m_controlValue = other.m_controlValue;
            return *this;
        }

        Object(Object&& other) noexcept
            : m_countStats(other.m_countStats)
        {
            if(m_countStats)
            {
                ++s_constructTotalCount;
                ++s_instanceCurrentCount;
            }

            *this = Move(other);
        }

        Object& operator=(Object&& other) noexcept
        {
            if(m_countStats)
            {
                ++s_moveTotalCount;
            }

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

        bool operator==(const u64 controlValue) const
        {
            return m_controlValue == controlValue;
        }

        static u64 GetInstanceCurrentCount()
        {
            return s_instanceCurrentCount;
        }

        static u64 GetConstructTotalCount()
        {
            return s_constructTotalCount;
        }

        static u64 GetDestructTotalCount()
        {
            return s_destructTotalCount;
        }

        static u64 GetCopyTotalCount()
        {
            return s_copyTotalCount;
        }

        static u64 GetMoveTotalCount()
        {
            return s_moveTotalCount;
        }
    };

    class ObjectDerived final : public Object
    {
    public:
        explicit ObjectDerived(const u64 controlValue = 0, bool countStats = true)
            : Object(controlValue, countStats)
        {
        }

        bool IsDerived() const override
        {
            return true;
        }
    };
}
