#pragma once

#include "Allocator.hpp"

namespace Memory
{
    bool Setup();

    Allocator& GetDefaultAllocator();
}
