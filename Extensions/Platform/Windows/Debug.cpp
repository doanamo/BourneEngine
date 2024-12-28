#include "Shared.hpp"
#include "Platform/Debug.hpp"
#include "Includes.hpp"

bool Debug::IsDebuggerPresent()
{
    return Win32::IsDebuggerPresent();
}

void Debug::DebuggerPrint(const char* message)
{
    // This call is thread safe, but does not guarantee that the message will be
    // printed in order when called from different threads. Implementing a custom
    // locking mechanism does not guarantee that the message will be printed in
    // order either, so it is not worth the effort.
    Win32::OutputDebugStringA(message);
}
