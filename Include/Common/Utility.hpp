#pragma once

#include "Common/Types.hpp"

template <typename Type, u64 Size>
constexpr u64 StaticArraySize(Type(&)[Size])
{
    return Size;
}
