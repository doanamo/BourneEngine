#pragma once

template<typename Type>
void InsertionSort(Type* begin, Type* end)
{
    ASSERT(begin && end);
    ASSERT(begin <= end);

    const u64 length = end - begin;

    u64 i = 1;
    while(i < length)
    {
        Type temp = Move(begin[i]);
        u64 j = i;

        while(j > 0 && temp < begin[j - 1])
        {
            begin[j] = Move(begin[j - 1]);
            --j;
        }

        begin[j] = Move(temp);
        ++i;
    }
}
