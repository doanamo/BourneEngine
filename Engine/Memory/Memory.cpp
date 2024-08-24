#include "Shared.hpp"
#include "Memory.hpp"

Memory::DefaultAllocator g_defaultAllocator;

Memory::DefaultAllocator& Memory::GetDefaultAllocator()
{
    return g_defaultAllocator;
}
