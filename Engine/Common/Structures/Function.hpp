#pragma once

#include "Memory/Memory.hpp"

template<typename Type>
class Function;

template<typename ReturnType, typename... Arguments>
class Function<ReturnType(Arguments...)> final
{
private:
    using InstancePtr = void*;
    using InvokerPtr = ReturnType(*)(InstancePtr, Arguments...);
    using CopierPtr = InstancePtr(*)(InstancePtr);
    using DeleterPtr = void(*)(InstancePtr);

    InstancePtr m_instance = nullptr;
    InvokerPtr m_invoker = nullptr;
    CopierPtr m_copier = nullptr;
    DeleterPtr m_deleter = nullptr;

    static ReturnType FunctionPointerInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return static_cast<ReturnType(*)(Arguments...)>(instance)(std::forward<Arguments>(arguments)...);
    }

    template<ReturnType(*FunctionType)(Arguments...)>
    static ReturnType StaticFunctionInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance == nullptr);
        return FunctionType(std::forward<Arguments>(arguments)...);
    }

    template<class InstanceType, auto Method>
    static ReturnType MutableMethodInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return (static_cast<InstanceType*>(instance)->*Method)(std::forward<Arguments>(arguments)...);
    }

    template<class InstanceType, auto Method>
    static ReturnType ConstMethodInvoker(const InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return (static_cast<const InstanceType*>(instance)->*Method)(std::forward<Arguments>(arguments)...);
    }

    template<class FunctorType>
    static ReturnType FunctorInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return (*static_cast<FunctorType*>(instance))(std::forward<Arguments>(arguments)...);
    }

public:
    Function() = default;
    ~Function()
    {
        ClearBinding();
    }

    Function(const Function& other)
    {
        *this = other;
    }

    Function& operator=(const Function& other)
    {
        ASSERT_SLOW(&other != this);

        if(other.m_copier)
        {
            m_instance = other.m_copier(other.m_instance);
        }
        else
        {
            m_instance = other.m_instance;
        }

        m_invoker = other.m_invoker;
        m_copier = other.m_copier;
        m_deleter = other.m_deleter;

        return *this;
    }

    Function(Function&& other) noexcept
        : Function()
    {
        *this = std::move(other);
    }

    Function& operator=(Function&& other) noexcept
    {
        ASSERT_SLOW(&other != this);

        ClearBinding();

        m_instance = other.m_instance;
        other.m_instance = nullptr;

        m_invoker = other.m_invoker;
        other.m_invoker = nullptr;

        m_copier = other.m_copier;
        other.m_copier = nullptr;

        m_deleter = other.m_deleter;
        other.m_deleter = nullptr;

        return *this;
    }

    template<ReturnType(*FunctionType)(Arguments...)>
    void Bind()
    {
        ClearBinding();

        m_instance = nullptr;
        m_invoker = &StaticFunctionInvoker<FunctionType>;
    }

    template<typename FunctionType>
    Function(FunctionType function)
    {
        Bind(std::forward<FunctionType>(function));
    }

    template<typename FunctionType>
    Function& operator=(FunctionType function)
    {
        Bind(std::forward<FunctionType>(function));
        return *this;
    }

    Function& operator=(std::nullptr_t)
    {
        Bind(nullptr);
        return *this;
    }

    void Bind(std::nullptr_t)
    {
        ClearBinding();
    }

    template<auto MethodType, class InstanceType>
    void Bind(InstanceType* instance)
    {
        ClearBinding();

        ASSERT(instance);
        m_instance = static_cast<void*>(instance);
        m_invoker = &MutableMethodInvoker<InstanceType, MethodType>;
    }

    template<auto MethodType, class InstanceType>
    void Bind(const InstanceType* instance)
    {
        ClearBinding();

        ASSERT(instance);
        m_instance = const_cast<void*>(static_cast<const void*>(instance));
        m_invoker = &ConstMethodInvoker<InstanceType, MethodType>;
    }

    template<typename FunctionType>
    void Bind(FunctionType function)
    {
        static_assert(std::is_invocable_r_v<ReturnType, FunctionType, Arguments...>,
            "Provided function argument is not invocable by this type!");

        ClearBinding();

        if constexpr(std::is_convertible_v<FunctionType, ReturnType(*)(Arguments...)>)
        {
            ASSERT(function != nullptr);
            m_instance = static_cast<void*>(function);
            m_invoker = &FunctionPointerInvoker;
        }
        else
        {
            m_instance = Memory::New<FunctionType>(std::forward<FunctionType>(function));
            m_invoker = &FunctorInvoker<FunctionType>;

            m_copier = [](void* instance) -> void*
            {
                ASSERT_SLOW(instance != nullptr);
                return Memory::New<FunctionType>(*static_cast<FunctionType*>(instance));
            };

            m_deleter = [](void* instance) -> void
            {
                ASSERT_SLOW(instance != nullptr);
                Memory::Delete<FunctionType>(static_cast<FunctionType*>(instance));
            };
        }
    }

    auto Invoke(Arguments... arguments)
    {
        ASSERT(m_invoker, "Function is not bound!");
        return Invoke(std::is_void<ReturnType>{}, std::forward<Arguments>(arguments)...);
    }

    auto operator()(Arguments... arguments)
    {
        return Invoke(std::forward<Arguments>(arguments)...);
    }

    bool IsBound()
    {
        return m_invoker != nullptr;
    }

private:
    void ClearBinding()
    {
        if(m_deleter)
        {
            m_deleter(m_instance);
            m_copier = nullptr;
            m_deleter = nullptr;
        }

        m_instance = nullptr;
        m_invoker = nullptr;

        ASSERT_SLOW(m_copier == nullptr);
        ASSERT_SLOW(m_deleter == nullptr);
    }

    ReturnType Invoke(std::false_type, Arguments... arguments)
    {
        return m_invoker(m_instance, std::forward<Arguments>(arguments)...);
    }

    void Invoke(std::true_type, Arguments... arguments)
    {
        m_invoker(m_instance, std::forward<Arguments>(arguments)...);
    }
};
