#include "Shared.hpp"
#include "Standard.hpp"

void* Memmem(const void* haystack, u64 haystackSize, const void* needle, u64 needleSize)
{
    if(needleSize == 0)
        return nullptr;

    const u64* iterator = (u64*)haystack;
    while(true)
    {
        const u64 sizeRemaining = haystackSize - (iterator - haystack);
        if(sizeRemaining < needleSize)
            break;

        iterator = (u64*)std::memchr(iterator, *(u8*)needle, sizeRemaining - needleSize + 1);
        if(!iterator)
            break;

        if(!std::memcmp(iterator, needle, needleSize))
            return (void*)iterator;

        iterator++;
    }

    return nullptr;
}
