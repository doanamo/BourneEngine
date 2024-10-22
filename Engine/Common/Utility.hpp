#pragma once

#include "Defines.hpp"
#include "Debug/Assert.hpp"

template <typename Type, u64 Size>
constexpr u64 ArraySize(Type(&)[Size])
{
    return Size;
}

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

constexpr u32 IsPow2(u32 x)
{
    return (x & (x - 1)) == 0;
}

constexpr u64 IsPow2(u64 x)
{
    return (x & (x - 1)) == 0;
}

constexpr u32 NextPow2(u32 x)
{
    ASSERT(x < (1u << 31), "Overflow");
    return x == 0 ? 1 : 1u << (32 - std::countl_zero(x));
}

constexpr u64 NextPow2(u64 x)
{
    ASSERT(x < (1ull << 63), "Overflow");
    return x == 0 ? 1 : 1ull << (64 - std::countl_zero(x));
}

template<typename Type>
constexpr bool NearlyEqual(Type a, Type b, Type epsilon)
{
    static_assert(std::is_floating_point<Type>::value);
    return std::abs(a - b) < epsilon;
}
