#pragma once

#include "Common/Containers/Array.hpp"

// #todo: Write unit tests.
class DelegateHandle final
{
    // #todo: Add debug checks for making sure this handle is used only with delegate that spawned it.
    //        Can't ure pointers for it because delegate itself may be moved in memory.

    constexpr static u32 Invalid = -1;
    u32 m_index = Invalid;

public:
    DelegateHandle() = default;
    DelegateHandle(u32 index)
        : m_index(index)
    {
    }

    bool IsValid() const
    {
        return m_index != Invalid;
    }

    u32 GetIndex() const
    {
        ASSERT(IsValid());
        return m_index;
    }

    void Invalidate()
    {
        m_index = Invalid;
    }
};

template<typename FunctionType, typename Allocator = Memory::Allocators::Default>
class Delegate;

template<typename Allocator, typename ReturnType, typename... Arguments>
class Delegate<ReturnType(Arguments...), Allocator> final
{
    static_assert(std::is_void_v<ReturnType>, "Only void return functions are supported");
    using FunctionType = Function<ReturnType(Arguments...)>;

    Array<FunctionType, Allocator> m_functions;

public:
    Delegate() = default;
    ~Delegate() = default;

    DelegateHandle Add(FunctionType&& function)
    {
        auto FindUnusedSlot = [](const FunctionType& function)
        {
            return !function.IsBound();
        };

        if(auto unusedSlotIndex = m_functions.FindIndexPredicate(FindUnusedSlot))
        {
            m_functions[*unusedSlotIndex] = Forward<FunctionType>(function);
            return DelegateHandle(*unusedSlotIndex);
        }
        else
        {
            m_functions.Add(Forward<FunctionType>(function));
            return DelegateHandle(m_functions.GetSize() - 1);
        }
    }

    bool Remove(DelegateHandle& handle)
    {
        if(!handle.IsValid())
        {
            LOG_WARNING("Attempted to remove invalid delegate handle");
            return false;
        }

        ASSERT_SLOW(handle.GetIndex() < m_functions.GetSize());
        m_functions[handle.GetIndex()].Unbind();
        handle.Invalidate();

        // #todo: Free memory for unbound tail elements? May be slow for constant adds/removes. Needs ShrinkToFit?

        return true;
    }

    void Broadcast(Arguments... arguments)
    {
        for(auto& function : m_functions)
        {
            if(function.IsBound())
            {
                function(Forward<Arguments>(arguments)...);
            }
        }
    }
};
