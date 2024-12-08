#include "Shared.hpp"
#include "Standard.hpp"

const void* Memmem(const void* haystack, u64 haystackSize, const void* needle, u64 needleSize)
{
    if(haystack == nullptr || haystackSize == 0)
        return nullptr;

    if(needle == nullptr || needleSize == 0)
        return nullptr;

    const u64* iterator = static_cast<const u64*>(haystack);
    while(true)
    {
        const u64 sizeRemaining = haystackSize - (iterator - haystack);
        if(sizeRemaining < needleSize)
            break;

        iterator = static_cast<const u64*>(std::memchr(iterator,
            *static_cast<const u8*>(needle), sizeRemaining - needleSize + 1));

        if(iterator == nullptr)
            break;

        if(std::memcmp(iterator, needle, needleSize) == 0)
            return iterator;

        iterator++;
    }

    return nullptr;
}
