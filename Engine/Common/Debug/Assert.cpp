#include "Shared.hpp"
#include "Assert.hpp"

static std::atomic<bool> g_handlingAssert = false;
static AssertCallback* g_assertCallback = nullptr;

void HandleAssert(const char* file, u32 line, const char* message, ...)
{
    if(g_handlingAssert.exchange(true))
    {
        // Already handling an assert
        Thread::SleepForever();
    }

    va_list arguments;
    va_start(arguments, message);
    OnAssertCallback(file, line, message, arguments);
    va_end(arguments);

    Debug::Abort();
}

void OnAssertCallback(const char* file, u32 line, const char* message, va_list arguments)
{
    if(g_assertCallback)
    {
        g_assertCallback(file, line, message, arguments);
    }
}

void SetAssertCallback(AssertCallback* callback)
{
    ASSERT(!g_assertCallback);
    g_assertCallback = callback;
}
