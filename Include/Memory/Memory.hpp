#pragma once

class Allocator;

namespace Memory
{
    void Setup();

    Allocator& GetAllocator();
}
