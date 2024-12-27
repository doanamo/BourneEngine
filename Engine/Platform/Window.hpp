#pragma once

#if defined(PLATFORM_WINDOWS)
    #include "Windows/Includes.hpp"
#elif defined(PLATFORM_LINUX)
    #include "Linux/Includes.hpp"
#endif

namespace Platform
{
    class Window final
    {
        UniquePtr<void> m_erased;

        String m_title;
        u32 m_width = 0;
        u32 m_height = 0;
        bool m_open = false;

        struct Private
        {
        #if defined(PLATFORM_WINDOWS)
            HWND handle = nullptr;
        #endif
        } m_private;

    public:
        enum class OpenError
        {
            CreateWindowFailed,
        };

        using OpenResult = Result<void, OpenError>;

        Window() = default;
        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        OpenResult Open(const char* title, u32 width, u32 height);
        void ProcessEvents();
        void Close();

        void SetTitle(const char* title);

        bool IsOpen() const
        {
            return m_open;
        }

        u32 GetWidth() const
        {
            return m_width;
        }

        u32 GetHeight() const
        {
            return m_height;
        }

        const Private& GetPrivate() const
        {
            return m_private;
        }

    private:
        OpenResult OnOpen();
        void OnClose();
        void OnProcessEvents();
        void OnSetTitle();

    #if defined(PLATFORM_WINDOWS)
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    #endif
    };
}
