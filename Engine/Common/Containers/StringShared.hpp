#pragma once

template<typename CharType>
class StringViewBase;

template<typename StringType, typename CharType>
class StringShared
{
protected:
    const char* GetData() const
    {
        return static_cast<const StringType*>(this)->GetData();
    }

    u64 GetLength() const
    {
        return static_cast<const StringType*>(this)->GetLength();
    }

    bool IsNullTerminated() const
    {
        return static_cast<const StringType*>(this)->IsNullTerminated();
    }

public:
    bool IsEmpty() const
    {
        return GetLength() == 0;
    }

    u64 GetCharSize() const
    {
        return sizeof(CharType);
    }

    const CharType* GetBeginPtr() const
    {
        ASSERT(GetData() != nullptr);
        return GetData();
    }

    const CharType* GetEndPtr() const
    {
        ASSERT(GetData() != nullptr);
        return GetData() + GetLength();
    }

    Optional<u64> FindIndex(const StringViewBase<CharType>& other) const
    {
        const void* result = memmem(GetData(), GetLength(), other.GetData(), other.GetLength());
        if(result == nullptr)
            return {};

        return static_cast<const CharType*>(result) - GetData();
    }

    bool StartsWith(const StringViewBase<CharType>& other) const
    {
        if(other.GetLength() > GetLength())
            return false;

        return std::memcmp(GetData(), other.GetData(), other.GetLength()) == 0;
    }

    bool EndsWith(const StringViewBase<CharType>& other) const
    {
        if(other.GetLength() > GetLength())
            return false;

        const u64 offset = GetLength() - other.GetLength();
        return std::memcmp(GetData() + offset, other.GetData(), other.GetLength()) == 0;
    }

    StringViewBase<CharType> SubString(u64 start, u64 end) const
    {
        start = std::min(start, GetLength());
        end = std::min(end, GetLength());
        return { GetData() + start, end - start };
    }

    StringViewBase<CharType> SubStringChopLeft(const u64 count) const
    {
        const u64 length = std::min(count, GetLength());
        return { GetData(), length };
    }

    StringViewBase<CharType> SubStringChopRight(const u64 count) const
    {
        const u64 offset = GetLength() - std::min(count, GetLength());
        const u64 length = GetLength() - offset;
        return { GetData() + offset, length };
    }

    StringViewBase<CharType> SubStringTrimLeft(const u64 count) const
    {
        const u64 offset = std::min(count, GetLength());
        const u64 length = GetLength() - offset;
        return { GetData() + offset, length };
    }

    StringViewBase<CharType> SubStringTrimRight(const u64 count) const
    {
        const u64 length = GetLength() - std::min(count, GetLength());
        return { GetData(), length };
    }

    StringViewBase<CharType> SubStringLeftAt(const u64 index) const
    {
        const u64 length = std::min(index, GetLength());
        return { GetData(), length };
    }

    StringViewBase<CharType> SubStringRightAt(const u64 index) const
    {
        const u64 offset = std::min(index, GetLength());
        const u64 length = GetLength() - offset;
        return { GetData() + offset, length };
    }

    bool operator==(const CharType* other) const
    {
        if(GetLength() != std::strlen(other))
            return false;

        return std::memcmp(GetData(), other, GetLength() * sizeof(CharType)) == 0;
    }

    bool operator!=(const CharType* other) const
    {
        if(GetLength() == std::strlen(other))
            return false;

        return std::memcmp(GetData(), other, GetLength() * sizeof(CharType)) != 0;
    }

    bool operator==(const StringViewBase<CharType>& other) const
    {
        if(GetLength() != other.GetLength())
            return false;

        return std::memcmp(GetData(), other.GetData(), GetLength() * sizeof(CharType)) == 0;
    }

    bool operator!=(const StringViewBase<CharType>& other) const
    {
        if(GetLength() == other.GetLength())
            return false;

        return std::memcmp(GetData(), other.GetData(), GetLength() * sizeof(CharType)) != 0;
    }

    const CharType* operator*() const
    {
        ASSERT(GetData() != nullptr);
        ASSERT(IsNullTerminated());
        return GetData();
    }

    const CharType& operator[](const u64 index) const
    {
        ASSERT(GetData() != nullptr);
        ASSERT(index < GetLength(), "Out of bounds access with %llu index and %llu length", index, GetLength());
        return GetData()[index];
    }
};
