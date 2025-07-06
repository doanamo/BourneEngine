#pragma once

#include "Config.hpp"
#include "ExitCodes.hpp"

class Application
{
public:
    virtual ~Application() = default;

    static UniquePtr<Application> Create();
    static const char* GetName();

    virtual Config GetConfig() = 0;

    virtual bool OnSetup()
    {
        return true;
    };

    virtual Optional<ExitCode> OnRun()
    {
        return {};
    };

    virtual void OnUpdate(float deltaTime)
    {
    };

    virtual void OnDraw(float alphaTime)
    {
    };
};

#define DEFINE_APPLICATION_NAME(ApplicationName, ApplicationType) \
    const char* Application::GetName() \
    { \
        return ApplicationName; \
    }

#define DEFINE_APPLICATION_CREATE(ApplicationType) \
    UniquePtr<Application> Application::Create() \
    { \
        return Memory::New<ApplicationType>(); \
    }

#define DEFINE_PRIMARY_APPLICATION(ApplicationName, ApplicationType) \
    DEFINE_APPLICATION_NAME(ApplicationName, ApplicationType) \
    DEFINE_APPLICATION_CREATE(ApplicationType)
