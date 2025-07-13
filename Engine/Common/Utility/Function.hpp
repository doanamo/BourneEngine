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

    struct Descriptor
    {
        InvokerPtr invoker = nullptr;
        CopierPtr copier = nullptr;
        DeleterPtr deleter = nullptr;
    };

    const Descriptor* m_descriptor = nullptr;
    InstancePtr m_instance = nullptr;

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
        DeleteInstance();
    }

    Function(const Function& other)
    {
        *this = other;
    }

    Function& operator=(const Function& other)
    {
        ASSERT_SLOW(&other != this);
        DeleteInstance();

        ASSERT_SLOW(m_instance == nullptr);
        if(other.m_descriptor && other.m_descriptor->copier)
        {
            m_instance = other.m_descriptor->copier(other.m_instance);
        }
        else
        {
            m_instance = other.m_instance;
        }

        m_descriptor = other.m_descriptor;

        return *this;
    }

    Function(Function&& other)
    {
        *this = Move(other);
    }

    Function& operator=(Function&& other)
    {
        ASSERT_SLOW(&other != this);
        DeleteInstance();

        m_instance = other.m_instance;
        other.m_instance = nullptr;

        m_descriptor = other.m_descriptor;
        other.m_descriptor = nullptr;

        return *this;
    }

    template<typename CallableType>
    requires (!std::same_as<std::decay_t<CallableType>, Function>)
    Function(CallableType&& function)
    {
        BindCallableNoClear(Forward<CallableType>(function));
    }

    template<typename CallableType>
    requires (!std::same_as<std::decay_t<CallableType>, Function>)
    Function& operator=(CallableType&& function)
    {
        Bind(Forward<CallableType>(function));
        return *this;
    }

    void Unbind()
    {
        ClearBinding();
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
        DeleteInstance();

        static const Descriptor descriptor =
        {
            .invoker = &FunctionTypeInvoker<FunctionType>,
            .copier = nullptr,
            .deleter = nullptr,
        };

        m_descriptor = &descriptor;
        m_instance = nullptr;
    }

    template<auto MethodType, class InstanceType>
    void Bind(InstanceType* instance)
    {
        ASSERT(instance);
        DeleteInstance();

        static const Descriptor descriptor =
        {
            .invoker = &MutableMethodInvoker<InstanceType, MethodType>,
            .copier = nullptr,
            .deleter = nullptr,
        };

        m_descriptor = &descriptor;
        m_instance = static_cast<void*>(instance);
    }

    template<auto MethodType, class InstanceType>
    void Bind(const InstanceType* instance)
    {
        ASSERT(instance);
        DeleteInstance();

        static const Descriptor descriptor =
        {
            .invoker = &ConstMethodInvoker<InstanceType, MethodType>,
            .copier = nullptr,
            .deleter = nullptr,
        };

        m_descriptor = &descriptor;
        m_instance = const_cast<void*>(static_cast<const void*>(instance));
    }

    template<typename CallableType>
    void Bind(CallableType&& function)
    {
        DeleteInstance();
        BindCallableNoClear(Forward<CallableType>(function));
    }

    auto Invoke(Arguments... arguments)
    {
        ASSERT(IsBound(), "Function is not bound");
        return Invoke(std::is_void<ReturnType>{}, Forward<Arguments>(arguments)...);
    }

    auto operator()(Arguments... arguments)
    {
        ASSERT(IsBound(), "Function is not bound");
        return Invoke(Forward<Arguments>(arguments)...);
    }

    bool IsBound() const
    {
        return m_descriptor != nullptr;
    }

    explicit operator bool() const
    {
        return IsBound();
    }

private:
    void DeleteInstance()
    {
        if(m_descriptor && m_descriptor->deleter)
        {
            ASSERT_SLOW(m_instance);
            m_descriptor->deleter(m_instance);
            m_instance = nullptr;
        }
    }

    void ClearBinding()
    {
        DeleteInstance();
        m_descriptor = nullptr;
        m_instance = nullptr;
    }

    template<typename CallableType>
    requires (!std::same_as<std::decay_t<CallableType>, Function>)
    void BindCallableNoClear(CallableType&& function)
    {
        static_assert(std::is_invocable_r_v<ReturnType, CallableType, Arguments...>,
            "Provided function argument is not invocable by this type");

        if constexpr(std::is_pointer_v<std::decay_t<CallableType>> &&
            std::is_function_v<std::remove_pointer_t<CallableType>>)
        {
            ASSERT(function != nullptr);

            static const Descriptor descriptor =
            {
                .invoker = &FunctionPointerInvoker,
                .copier = nullptr,
                .deleter = nullptr,
            };

            m_descriptor = &descriptor;
            m_instance = reinterpret_cast<void*>(function);
        }
        else if constexpr(std::is_convertible_v<CallableType, ReturnType(*)(Arguments...)>)
        {
            ASSERT(function != nullptr);

            static const Descriptor descriptor =
            {
                .invoker = &FunctionPointerInvoker,
                .copier = nullptr,
                .deleter = nullptr,
            };

            m_descriptor = &descriptor;
            m_instance = reinterpret_cast<void*>(static_cast<ReturnType(*)(Arguments...)>(function));
        }
        else
        {
            using LambdaType = std::decay_t<CallableType>;

            static const Descriptor descriptor =
            {
                .invoker = &FunctorInvoker<LambdaType>,
                .copier = [](void* instance) -> void*
                {
                    ASSERT_SLOW(instance != nullptr);
                    return Memory::New<LambdaType>(*static_cast<LambdaType*>(instance));
                },
                .deleter = [](void* instance) -> void
                {
                    ASSERT_SLOW(instance != nullptr);
                    Memory::Delete<LambdaType>(static_cast<LambdaType*>(instance));
                },
            };

            m_descriptor = &descriptor;
            m_instance = Memory::New<LambdaType>(Forward<CallableType>(function));
        }
    }

    ReturnType Invoke(std::false_type, Arguments... arguments)
    {
        return m_descriptor->invoker(m_instance, Forward<Arguments>(arguments)...);
    }

    void Invoke(std::true_type, Arguments... arguments)
    {
        m_descriptor->invoker(m_instance, Forward<Arguments>(arguments)...);
    }
};

static_assert(sizeof(Function<void()>) == 16);
