#pragma once

class LoggerOutputDebugger final
{
public:
    LoggerOutputDebugger();
    void Output(const char* text);

private:
    bool m_isDebuggerPresent = false;
};
