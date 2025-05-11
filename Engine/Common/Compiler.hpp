#pragma once

#include <immintrin.h>

#define STRINGIFY(x) #x
#define EXPAND(x) x
#define CONCAT_INNER(a, b) a ## b
#define CONCAT(a, b) CONCAT_INNER(a, b)

#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

#if defined(COMPILER_MSVC)
    #define INTRINSIC [[msvc::intrinsic]]
#else
    #define INTRINSIC
#endif

#if defined(COMPILER_MSVC)
    #pragma warning(error: 4714)
    #define FORCE_INLINE __forceinline
#elif defined(COMPILER_CLANG)
    #define FORCE_INLINE [[gnu::always_inline]] [[gnu::gnu_inline]] extern inline
#elif defined(COMPILER_GCC)
    #define FORCE_INLINE [[gnu::always_inline]] inline
#else
    #define FORCE_INLINE
#endif

template<typename Type>
[[nodiscard]] INTRINSIC Type&& Forward(std::remove_reference_t<Type>& value)
{
    return static_cast<Type&&>(value);
}
template<typename Type>
[[nodiscard]] INTRINSIC Type&& Forward(std::remove_reference_t<Type>&& value)
{
    static_assert(!std::is_lvalue_reference_v<Type>, "Forward called on lvalue reference");
    return static_cast<Type&&>(value);
}

template<typename Type>
[[nodiscard]] INTRINSIC constexpr std::remove_reference_t<Type>&& Move(Type&& value)
{
    static_assert(std::is_lvalue_reference_v<Type>, "Move called on rvalue reference");
    static_assert(!std::is_const_v<Type>, "Move called on const value");
    return static_cast<std::remove_reference_t<Type>&&>(value);
}

template<typename Type>
[[nodiscard]] INTRINSIC constexpr std::remove_reference_t<Type>&& MoveWeak(Type&& value)
{
    return static_cast<std::remove_reference_t<Type>&&>(value);
}
