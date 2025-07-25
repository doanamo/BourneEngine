#pragma once

#include "Common/Debug/Assert.hpp"
#include "Common/Utility/Utility.hpp"

namespace Memory
{
    constexpr u8 UninitializedPattern = 0xBE;
    constexpr u8 DestructedPattern = 0xDE;
    constexpr u8 FreedPattern = 0xFE;

    inline void MarkUninitialized(void* memory, const u64 size)
    {
    #if ENABLE_MEMORY_FILL
        std::memset(memory, UninitializedPattern, size);
    #endif
    }

    inline void MarkDestructed(void* memory, const u64 size)
    {
    #if ENABLE_MEMORY_FILL
        std::memset(memory, DestructedPattern, size);
    #endif
    }

    inline void MarkFreed(void* memory, const u64 size)
    {
    #if ENABLE_MEMORY_FILL
        std::memset(memory, FreedPattern, size);
    #endif
    }

    constexpr u64 AlignSize(const u64 size, const u64 alignment)
    {
        ASSERT(IsPow2(alignment));
        return (size + (alignment - 1)) & ~(alignment - 1);
    }

    template<typename Type>
    struct ObjectStorage
    {
        // Aligned bytes that can store an instance of a given type.
        // Useful when you need to avoid implicit construction/destruction.
        alignas(Type) u8 bytes[sizeof(Type)];
    };

    static_assert(sizeof(ObjectStorage<u8>) == sizeof(u8));
    static_assert(sizeof(ObjectStorage<u16>) == sizeof(u16));
    static_assert(sizeof(ObjectStorage<u32>) == sizeof(u32));
    static_assert(sizeof(ObjectStorage<u64>) == sizeof(u64));
}
