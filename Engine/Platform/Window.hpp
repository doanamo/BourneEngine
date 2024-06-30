#pragma once

class Window
{
public:
    static UniquePtr<Window> Create();

    virtual ~Window() = default;
    virtual bool Setup() = 0;
};
