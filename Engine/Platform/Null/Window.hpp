#pragma once

#include "Platform/WindowEvents.hpp"

namespace Platform::Detail
{
    class Window
    {
    public:
        static void ProcessEvents();

        Window() = default;
        ~Window() = default;

        void SetOnCloseEvent(OnWindowCloseFunction&& function);
        void SetOnResizeEvent(OnWindowResizeFunction&& function);
        bool Setup(const StringView& title, u32& width, u32& height);

        void Resize(u32 width, u32 height);
        void UpdateTitle(const StringView& title);
        void UpdateVisibility(bool visible);
    };
}
