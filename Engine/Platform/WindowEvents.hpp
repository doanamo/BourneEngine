#pragma once

namespace Platform::Detail
{
    using OnWindowCloseFunction = Function<void()>;
    using OnWindowResizeFunction = Function<void(u32 width, u32 height)>;
}
