#pragma once

#include <immintrin.h>

#define STRINGIFY(x) #x
#define EXPAND(x) x
#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

template<typename Type>
[[nodiscard]] Type&& Forward(std::remove_reference_t<Type>& value)
{
    return static_cast<Type&&>(value);
}
template<typename Type>
[[nodiscard]] Type&& Forward(std::remove_reference_t<Type>&& value)
{
    static_assert(!std::is_lvalue_reference_v<Type>, "Forward called on lvalue reference");
    return static_cast<Type&&>(value);
}

template<typename Type>
[[nodiscard]] constexpr std::remove_reference_t<Type>&& Move(Type&& value)
{
    static_assert(std::is_lvalue_reference_v<Type>, "Move called on rvalue reference");
    static_assert(!std::is_const_v<Type>, "Move called on const value");
    return static_cast<std::remove_reference_t<Type>&&>(value);
}

template<typename Type>
[[nodiscard]] constexpr std::remove_reference_t<Type>&& MoveWeak(Type&& value)
{
    return static_cast<std::remove_reference_t<Type>&&>(value);
}
