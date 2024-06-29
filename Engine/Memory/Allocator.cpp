#include "Shared.hpp"
#include "Allocator.hpp"
#include "DefaultAllocator.hpp"

Allocator& Allocator::GetDefault()
{
    return DefaultAllocator::Get();
}
