#include "Memory/Shared.hpp"
#include "Memory/Memory.hpp"
#include "Memory/DefaultAllocator.hpp"

static DefaultAllocator* g_defaultAllocator = nullptr;

void Memory::Setup()
{
    static DefaultAllocator defaultAllocator;
    g_defaultAllocator = &defaultAllocator;
}

Allocator& Memory::GetDefaultAllocator()
{
    ASSERT(g_defaultAllocator);
    return *g_defaultAllocator;
}
