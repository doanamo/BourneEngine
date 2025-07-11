#pragma once

template<typename Type>
class Singleton
{
    friend Type;
    Singleton() = default;
    ~Singleton() = default;

public:
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Type& Get()
    {
        static Type instance;
        return instance;
    }
};