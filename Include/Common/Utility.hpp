#pragma once

#include "Common/Types.hpp"

template <typename Type, u64 Size>
constexpr u64 StaticArraySize(Type(&)[Size])
{
    return Size;
}

namespace Math
{
    template<typename Type>
    constexpr Type Min(Type a, Type b)
    {
        return a < b ? a : b;
    }

    template<typename Type>
    constexpr Type Max(Type a, Type b)
    {
        return a > b ? a : b;
    }

    template<typename Integer>
    constexpr bool IsPow2(Integer value)
    {
        return (value & (value - 1)) == 0;
    }

    constexpr u32 NextPow2(u32 value)
    {
        return value == 1 ? 1 : 1 << (32 - std::countl_zero(value - 1));
    }

    constexpr u64 NextPow2(u64 value)
    {
        return value == 1 ? 1 : 1 << (64 - std::countl_zero(value - 1));
    }
}
