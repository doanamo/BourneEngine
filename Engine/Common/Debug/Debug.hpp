#pragma once

#if defined(PLATFORM_WINDOWS)
    #define DEBUG_BREAK() \
        if(IsDebuggerPresent()) \
        { \
            __debugbreak();   \
        }
#elif defined(PLATFORM_LINUX)
    #include <csignal>
    #define DEBUG_BREAK() std::raise(SIGINT)
#else
    #error "Unknown platform"
#endif

#define DEBUG_ABORT() \
    DEBUG_BREAK(); \
    std::abort();
