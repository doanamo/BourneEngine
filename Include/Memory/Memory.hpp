#pragma once

#include "Memory/Allocator.hpp"

namespace Memory
{
    void Setup();

    Allocator& GetDefaultAllocator();
}
