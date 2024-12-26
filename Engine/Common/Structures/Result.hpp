#pragma once

namespace Detail
{
    template<typename Type = void>
    struct SuccessStorage
    {
        explicit SuccessStorage(const Type& value)
            : value(value)
        {
        }

        explicit SuccessStorage(Type&& value)
            : value(std::move(value))
        {
        }

        Type value;
    };

    template<>
    struct SuccessStorage<void>
    {
        Empty value;
    };

    template <typename Type = void>
    struct FailureStorage
    {
        explicit FailureStorage(const Type& value)
            : value(value)
        {
        }

        explicit FailureStorage(Type&& value)
            : value(std::move(value))
        {
        }

        Type value;
    };

    template<>
    struct FailureStorage<void>
    {
        Empty value;
    };
}

template<typename Type>
constexpr auto Success(Type&& value)
{
    return Detail::SuccessStorage<Type>(std::move(value));
}

constexpr auto Success()
{
    return Detail::SuccessStorage<void>();
}

template<typename Type>
constexpr auto Failure(Type&& value)
{
    return Detail::FailureStorage<Type>(std::move(value));
}

constexpr auto Failure()
{
    return Detail::FailureStorage<void>();
}

template<typename SuccessType, typename FailureType>
class Result
{
    using SuccessStorage = Detail::SuccessStorage<SuccessType>;
    using FailureStorage = Detail::FailureStorage<FailureType>;
    using StorageType = std::variant<SuccessStorage, FailureStorage>;

    static constexpr u64 StorageSuccessIndex = 0;
    static constexpr u64 StorageFailureIndex = 1;

    StorageType m_storage;

public:
    Result(SuccessStorage&& success)
        : m_storage(std::in_place_index<StorageSuccessIndex>, std::move(success))
    {
    }

    Result(FailureStorage&& failure)
        : m_storage(std::in_place_index<StorageFailureIndex>, std::move(failure))
    {
    }

    auto& Get()
    {
        return GetSuccess();
    }

    auto& GetSuccess()
    {
        ASSERT(IsSuccess(), "Invalid result unwrap!");
        return std::get<StorageSuccessIndex>(m_storage).value;
    }

    auto& GetFailure()
    {
        ASSERT(IsFailure(), "Invalid result unwrap!");
        return std::get<StorageFailureIndex>(m_storage).value;
    }

    const auto& Get() const
    {
        return GetSuccess();
    }

    const auto& GetSuccess() const
    {
        ASSERT(IsSuccess(), "Invalid result unwrap!");
        return std::get<StorageSuccessIndex>(m_storage).value;
    }

    const auto& GetFailure() const
    {
        ASSERT(IsFailure(), "Invalid result unwrap!");
        return std::get<StorageFailureIndex>(m_storage).value;
    }

    SuccessType Unwrap()
    {
        return UnwrapSuccess();
    }

    SuccessType UnwrapSuccess()
    {
        ASSERT(IsSuccess(), "Invalid result unwrap!");
        return std::move(std::get<StorageSuccessIndex>(m_storage).value);
    }

    FailureType UnwrapFailure()
    {
        ASSERT(IsFailure(), "Invalid result unwrap!");
        return std::move(std::get<StorageFailureIndex>(m_storage).value);
    }

    bool IsSuccess() const
    {
        return std::get_if<StorageSuccessIndex>(&m_storage) != nullptr;
    }

    bool IsFailure() const
    {
        return std::get_if<StorageFailureIndex>(&m_storage) != nullptr;
    }

    explicit operator bool() const
    {
        return IsSuccess();
    }
};
