#include "Shared.hpp"
#include "Window.hpp"

#ifdef PLATFORM_WINDOWS
    #include "Windows/Window.hpp"
#endif

UniquePtr<Window> Window::Create()
{
#ifdef PLATFORM_WINDOWS
    return MakeUnique<Windows::Window>();
#else
    #error Unsupported platform!
#endif
}

