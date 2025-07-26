#include "Shared.hpp"
#include "Engine/Engine.hpp"

class ExampleApplication final : public Application
{
public:
    Config GetConfig() override;

    bool OnSetup() override;
    void OnUpdate(float deltaTime) override;
    void OnDraw(float alphaTime) override;
};

DEFINE_PRIMARY_APPLICATION("Bourne Engine Example", ExampleApplication);

Config ExampleApplication::GetConfig()
{
    Config config;
    config.window.width = 1024;
    config.window.height = 576;
    config.render.software = false;
    return config;
}

bool ExampleApplication::OnSetup()
{
    return true;
}

void ExampleApplication::OnUpdate(float deltaTime)
{
}

void ExampleApplication::OnDraw(float alphaTime)
{
    // #todo: Draw a triangle.
}
