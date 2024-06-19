#pragma once

#include "Allocator.hpp"

namespace Memory
{
    void Setup();
    Allocator& GetDefaultAllocator();
}
