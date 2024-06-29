#pragma once

class LoggerOutputDebugger final
{
private:
    bool m_isDebuggerPresent = false;

public:
    LoggerOutputDebugger();
    void Output(const char* text);
};
