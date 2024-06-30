#pragma once

#include "Platform/Window.hpp"

namespace Windows
{
    class Window final : public ::Window
    {
    public:
        bool Setup() override;
    };
}
