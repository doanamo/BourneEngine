#pragma once

class Logger
{
public:
    static Logger& Get();

    void Log(const char* message, const char* source, u32 line);
};

#define LOG(message) Logger::Get().Log(message, __FILE__, __LINE__)
