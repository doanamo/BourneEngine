#pragma once

#include "Platform/WindowEvents.hpp"

namespace Platform::Detail
{
    class Window
    {
    public:
        Window() = default;
        ~Window() = default;

        void SetOnCloseEvent(OnWindowCloseFunction&& function);
        bool Setup(const StringView& title, u32& width, u32& height);

        void ProcessEvents();
        void SetSize(u32 width, u32 height);
        void SetTitle(const StringView& title);
        void SetVisibility(bool visible);

        void GetSize(u32& width, u32& height);
    };
}
