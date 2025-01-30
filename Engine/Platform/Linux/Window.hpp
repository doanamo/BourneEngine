#pragma once

namespace Platform
{
    struct WindowPrivate
    {
        xcb_screen_t* screen = nullptr;
        xcb_window_t window = XCB_NONE;
    };
}
