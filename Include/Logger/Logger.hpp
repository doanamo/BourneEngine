#pragma once

namespace Logger
{
    void Setup();
    void Log(const char* message, const char* source, u32 line);
};

#define LOG(message) Logger::Log(message, __FILE__, __LINE__)
