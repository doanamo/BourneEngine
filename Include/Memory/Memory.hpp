#pragma once

#include "Memory/Allocator.hpp"

namespace Memory
{
    bool Setup();

    Allocator& GetDefaultAllocator();
}
