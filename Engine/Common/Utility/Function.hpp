#pragma once

#include "Memory/Memory.hpp"

template<typename Type>
class Function;

template<typename ReturnType, typename... Arguments>
class Function<ReturnType(Arguments...)> final
{
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
        return reinterpret_cast<ReturnType(*)(Arguments...)>(instance)(Forward<Arguments>(arguments)...);
    }

    template<ReturnType(*FunctionType)(Arguments...)>
    static ReturnType FunctionTypeInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance == nullptr);
        return FunctionType(Forward<Arguments>(arguments)...);
    }

    template<class InstanceType, auto Method>
    static ReturnType MutableMethodInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return (static_cast<InstanceType*>(instance)->*Method)(Forward<Arguments>(arguments)...);
    }

    template<class InstanceType, auto Method>
    static ReturnType ConstMethodInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return (static_cast<const InstanceType*>(instance)->*Method)(Forward<Arguments>(arguments)...);
    }

    template<class FunctorType>
    static ReturnType FunctorInvoker(InstancePtr instance, Arguments... arguments)
    {
        ASSERT_SLOW(instance != nullptr);
        return (*static_cast<FunctorType*>(instance))(Forward<Arguments>(arguments)...);
    }

public:
    Function() = default;

    ~Function()
    {
        if(m_deleter)
        {
            m_deleter(m_instance);
        }
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
        *this = Move(other);
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

    template<typename CallableType>
    Function(CallableType&& function)
    {
        BindCallableNoClear(Forward<CallableType>(function));
    }

    template<typename CallableType>
    Function& operator=(CallableType&& function)
    {
        Bind(Forward<CallableType>(function));
        return *this;
    }

    void Bind(std::nullptr_t)
    {
        ClearBinding();
    }

    Function& operator=(std::nullptr_t)
    {
        ClearBinding();
        return *this;
    }

    template<ReturnType(*FunctionType)(Arguments...)>
    void Bind()
    {
        ClearBinding();

        m_instance = nullptr;
        m_invoker = &FunctionTypeInvoker<FunctionType>;
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

    template<typename CallableType>
    void Bind(CallableType&& function)
    {
        ClearBinding();
        BindCallableNoClear(Forward<CallableType>(function));
    }

    auto Invoke(Arguments... arguments)
    {
        ASSERT(m_invoker, "Function is not bound");
        return Invoke(std::is_void<ReturnType>{}, Forward<Arguments>(arguments)...);
    }

    auto operator()(Arguments... arguments)
    {
        return Invoke(Forward<Arguments>(arguments)...);
    }

    bool IsBound() const
    {
        return m_invoker != nullptr;
    }

    explicit operator bool() const
    {
        return IsBound();
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

    template<typename CallableType>
    void BindCallableNoClear(CallableType&& function)
    {
        static_assert(std::is_invocable_r_v<ReturnType, CallableType, Arguments...>,
            "Provided function argument is not invocable by this type");

        ASSERT_SLOW(m_instance == nullptr);
        ASSERT_SLOW(m_invoker == nullptr);
        ASSERT_SLOW(m_copier == nullptr);
        ASSERT_SLOW(m_deleter == nullptr);

        if constexpr(std::is_pointer_v<std::decay_t<CallableType>> && std::is_function_v<std::remove_pointer_t<CallableType>>)
        {
            ASSERT(function != nullptr);
            m_instance = reinterpret_cast<void*>(function);
            m_invoker = &FunctionPointerInvoker;
        }
        else if constexpr(std::is_convertible_v<CallableType, ReturnType(*)(Arguments...)>)
        {
            ASSERT(function != nullptr);
            m_instance = reinterpret_cast<void*>(static_cast<ReturnType(*)(Arguments...)>(function));
            m_invoker = &FunctionPointerInvoker;
        }
        else
        {
            using LambdaType = std::decay_t<CallableType>;
            m_instance = Memory::New<LambdaType>(Forward<CallableType>(function));
            m_invoker = &FunctorInvoker<LambdaType>;

            m_copier = [](void* instance) -> void*
            {
                ASSERT_SLOW(instance != nullptr);
                return Memory::New<LambdaType>(*static_cast<LambdaType*>(instance));
            };

            m_deleter = [](void* instance) -> void
            {
                ASSERT_SLOW(instance != nullptr);
                Memory::Delete<LambdaType>(static_cast<LambdaType*>(instance));
            };
        }
    }

    ReturnType Invoke(std::false_type, Arguments... arguments)
    {
        return m_invoker(m_instance, Forward<Arguments>(arguments)...);
    }

    void Invoke(std::true_type, Arguments... arguments)
    {
        m_invoker(m_instance, Forward<Arguments>(arguments)...);
    }
};
