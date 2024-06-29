#pragma once

class Allocator
{
public:
    static Allocator& GetDefault();

    template<typename Type>
    Type* Allocate(u64 count = 1)
    {
        return (Type*)Allocate(sizeof(Type) * count, alignof(Type));
    }

    template<typename Type>
    Type* Reallocate(Type* allocation, u64 count)
    {
        return (Type*)Reallocate(allocation, sizeof(Type) * count, alignof(Type));
    }

    virtual void* Allocate(u64 size, u32 alignment) = 0;
    virtual void* Reallocate(void* allocation, u64 size, u32 alignment) = 0;
    virtual void Deallocate(void* allocation) = 0;
};
