#pragma once

namespace Platform::Detail
{
    struct Window
    {
        xcb_screen_t* screen = nullptr;
        xcb_window_t window = XCB_NONE;
    };
}
