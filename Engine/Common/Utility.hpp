#pragma once

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

template<typename Integer>
constexpr bool IsPow2(Integer x)
{
    return (x & (x - 1)) == 0;
}

constexpr u32 NextPow2(u32 x)
{
    return x == 0 ? 1 : 1 << (32 - std::countl_zero(x));
}

constexpr u64 NextPow2(u64 x)
{
    return x == 0 ? 1 : 1 << (64 - std::countl_zero(x));
}
