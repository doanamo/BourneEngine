#include "Common/Shared.hpp"

static AssertCallback* g_assertCallback = nullptr;

void OnAssertCallback(const char* expression, const char* file, u32 line, const char* message)
{
    if(g_assertCallback)
    {
        g_assertCallback(expression, file, line, message);
    }
}

void SetAssertCallback(AssertCallback* callback)
{
    ASSERT(!g_assertCallback);
    g_assertCallback = callback;
}
