#pragma once

template<typename SuccessType, typename FailureType>
class Result
{
    struct SuccessTag {};
    struct FailureTag {};

    using SuccessStorage = std::conditional_t<std::is_void_v<SuccessType>, Empty, SuccessType>;
    using FailureStorage = std::conditional_t<std::is_void_v<FailureType>, Empty, FailureType>;
    using StorageType = std::variant<SuccessStorage, FailureStorage>;

    static constexpr u64 StorageSuccessIndex = 0;
    static constexpr u64 StorageFailureIndex = 1;

    StorageType m_storage;

    Result(SuccessTag, SuccessStorage&& success)
        : m_storage(std::in_place_index<StorageSuccessIndex>, std::move(success))
    {}

    Result(FailureTag, FailureStorage&& failure)
        : m_storage(std::in_place_index<StorageFailureIndex>, std::move(failure))
    {}
public:
    static auto Success()
    {
        static_assert(std::is_void_v<SuccessType>, "Non-void success result requires success argument!");
        return Result(SuccessTag{}, Empty{});
    }

    static auto Success(SuccessStorage&& success)
    {
        return Result(SuccessTag{}, std::move(success));
    }

    static auto Failure()
    {
        static_assert(std::is_void_v<FailureType>, "Non-void failure result requires failure argument!");
        return Result(FailureTag{}, Empty{});
    }

    static auto Failure(FailureStorage&& failure)
    {
        return Result(FailureTag{}, std::move(failure));
    }

    auto& Get()
    {
        return GetSuccess();
    }

    auto& GetSuccess()
    {
        ASSERT(IsSuccess(), "Invalid result unwrap!");
        return std::get<StorageSuccessIndex>(m_storage);
    }

    auto& GetFailure()
    {
        ASSERT(IsFailure(), "Invalid result unwrap!");
        return std::get<StorageFailureIndex>(m_storage);
    }

    const auto& Get() const
    {
        return GetSuccess();
    }

    const auto& GetSuccess() const
    {
        ASSERT(IsSuccess(), "Invalid result unwrap!");
        return std::get<StorageSuccessIndex>(m_storage);
    }

    const auto& GetFailure() const
    {
        ASSERT(IsFailure(), "Invalid result unwrap!");
        return std::get<StorageFailureIndex>(m_storage);
    }

    SuccessType Unwrap()
    {
        return UnwrapSuccess();
    }

    SuccessType UnwrapSuccess()
    {
        ASSERT(IsSuccess(), "Invalid result unwrap!");
        return std::move(std::get<StorageSuccessIndex>(m_storage));
    }

    FailureType UnwrapFailure()
    {
        ASSERT(IsFailure(), "Invalid result unwrap!");
        return std::move(std::get<StorageFailureIndex>(m_storage));
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

template<typename Type>
using SuccessResult = Result<Type, void>;

template<typename Type>
using FailureResult = Result<void, Type>;
