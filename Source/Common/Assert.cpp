#include "Common/Shared.hpp"

static std::atomic<bool> g_handlingAssert = false;
static AssertCallback* g_assertCallback = nullptr;

void HandleAssert(const char* expression, const char* message, const char* file, u32 line)
{
    while(g_handlingAssert.exchange(true))
    {
        Thread::Sleep(1);
    }

    OnAssertCallback(expression, message, __FILE__, __LINE__);
    Debug::Abort();
}

void OnAssertCallback(const char* expression, const char* message, const char* file, u32 line)
{
    if(g_assertCallback)
    {
        g_assertCallback(expression, message, file, line);
    }
}

void SetAssertCallback(AssertCallback* callback)
{
    ASSERT(!g_assertCallback);
    g_assertCallback = callback;
}
