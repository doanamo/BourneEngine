#pragma once

namespace Engine
{
    struct Config
    {
        const char* applicationName = nullptr;
        const char* const* commandLineArguments = nullptr;
        int commandLineArgumentCount = 0;
    };

    void Setup(const Config& config);
    const char* GetApplicationName();
}
