#include "Shared.hpp"
#include "Platform/Window.hpp"
#include "Includes.hpp"

xcb_connection_t* g_xcbConnection = nullptr;
u32 g_xcbConnectionReferences = 0;

xcb_atom_t g_xcbAtomUserData = XCB_NONE;
xcb_atom_t g_xcbAtomWMProtocols = XCB_NONE;
xcb_atom_t g_xcbAtomWMDeleteWindow = XCB_NONE;
xcb_atom_t g_xcbAtomWMName = XCB_NONE;
xcb_atom_t g_xcbAtomUTF8String = XCB_NONE;

struct WindowPrivate
{
    xcb_screen_t* screen = nullptr;
    xcb_window_t window = XCB_NONE;
};

static xcb_atom_t GetXCBAtom(const char* name, const bool create)
{
    const xcb_intern_atom_cookie_t cookie = xcb_intern_atom(
        g_xcbConnection, create ? 0 : 1, strlen(name), name);

    xcb_generic_error_t* error = nullptr;
    if(xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(g_xcbConnection, cookie, &error))
    {
        const xcb_atom_t atom = reply->atom;
        free(reply);
        return atom;
    }
    else
    {
        LOG_WARNING("Failed to retrieve XCB atom (error code: %i)", error->error_code);
        free(error);
        return XCB_NONE;
    }
}

static bool OpenXCBConnection()
{
    if(!g_xcbConnection)
    {
        ASSERT_SLOW(g_xcbConnectionReferences == 0);
        g_xcbConnection = xcb_connect(nullptr, nullptr);
        if(g_xcbConnection == nullptr)
            return false;
    }

    g_xcbAtomUserData = GetXCBAtom("USER_DATA", true);
    g_xcbAtomWMProtocols = GetXCBAtom("WM_PROTOCOLS", false);
    g_xcbAtomWMDeleteWindow = GetXCBAtom("WM_DELETE_WINDOW", false);
    g_xcbAtomWMName = GetXCBAtom("_NET_WM_NAME", false);
    g_xcbAtomUTF8String = GetXCBAtom("UTF8_STRING", false);

    ++g_xcbConnectionReferences;
    return true;
}

static void CloseXCBConnection()
{
    ASSERT(g_xcbConnectionReferences >= 1);
    g_xcbConnectionReferences--;

    if(g_xcbConnectionReferences == 0)
    {
        ASSERT_SLOW(g_xcbConnection);
        xcb_disconnect(g_xcbConnection);
        g_xcbConnection = nullptr;

        g_xcbAtomUserData = XCB_NONE;
        g_xcbAtomWMProtocols = XCB_NONE;
        g_xcbAtomWMDeleteWindow = XCB_NONE;
        g_xcbAtomWMName = XCB_NONE;
        g_xcbAtomUTF8String = XCB_NONE;
    }
}

static Platform::Window* GetWindowInstanceFromUserData(const xcb_window_t window)
{
    const xcb_get_property_cookie_t cookie = xcb_get_property(
        g_xcbConnection,
        false,
        window,
        g_xcbAtomUserData,
        XCB_ATOM_ATOM,
        0,2);

    xcb_generic_error_t* error;
    if(xcb_get_property_reply_t* reply = xcb_get_property_reply(g_xcbConnection, cookie, &error))
    {
        auto* instance = reinterpret_cast<Platform::Window*>(*static_cast<u64*>(xcb_get_property_value(reply)));
        free(reply);
        return instance;
    }
    else
    {
        LOG_WARNING("Failed to retrieve user data from XCB window (error code %i)", error->error_code);
        free(error);
        return nullptr;
    }
};

void Platform::Window::ProcessEvents()
{
    while(g_xcbConnection)
    {
        xcb_generic_event_t* event = xcb_poll_for_event(g_xcbConnection);
        if(event == nullptr)
            break;

        switch(event->response_type & 0x7f)
        {
        case XCB_CLIENT_MESSAGE:
            {
                const auto* clientMessageEvent = reinterpret_cast<xcb_client_message_event_t*>(event);
                if(Window* window = GetWindowInstanceFromUserData(clientMessageEvent->window))
                {
                    if(clientMessageEvent->type == g_xcbAtomWMProtocols &&
                        clientMessageEvent->data.data32[0] == g_xcbAtomWMDeleteWindow)
                    {
                        window->OnCloseEvent();
                    }
                }
            }
            break;

        case XCB_CONFIGURE_NOTIFY:
            {
                const auto* configureNotifyEvent = reinterpret_cast<xcb_configure_notify_event_t*>(event);
                if(Window* window = GetWindowInstanceFromUserData(configureNotifyEvent->window))
                {
                    window->OnResizeEvent(configureNotifyEvent->width, configureNotifyEvent->height);
                }
            }
            break;

        default:
            break;
        }

        free(event);
    }
}

bool Platform::Window::OnOpen()
{
    ASSERT_SLOW(!m_open);
    ASSERT_SLOW(!m_private);

    auto* windowPrivate = Memory::New<WindowPrivate>();
    m_private.Reset(windowPrivate,
        [](void* pointer)
        {
            Memory::Delete(static_cast<WindowPrivate*>(pointer));
        });

    if(!OpenXCBConnection())
    {
        LOG_ERROR("Failed to open XCB connection");
        return false;
    }

    SCOPE_GUARD([this]()
    {
        if(!m_open)
        {
            m_private.Reset();
            CloseXCBConnection();
        }
    });

    windowPrivate->screen = xcb_setup_roots_iterator(xcb_get_setup(g_xcbConnection)).data;
    if(!windowPrivate->screen)
    {
        LOG_ERROR("Failed to retrieve XCB screen");
        return false;
    }

    constexpr uint32_t windowValues[] =
    {
        XCB_EVENT_MASK_EXPOSURE |
        XCB_EVENT_MASK_STRUCTURE_NOTIFY |
        XCB_EVENT_MASK_KEY_PRESS |
        XCB_EVENT_MASK_KEY_RELEASE |
        XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_POINTER_MOTION
    };

    windowPrivate->window = xcb_generate_id(g_xcbConnection);
    xcb_void_cookie_t cookie = xcb_create_window_checked(
        g_xcbConnection,
        XCB_COPY_FROM_PARENT,
        windowPrivate->window,
        windowPrivate->screen->root,
        0, 0,
        m_width,
        m_height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        windowPrivate->screen->root_visual,
        XCB_CW_EVENT_MASK, windowValues);

    xcb_generic_error_t* error;
    if((error = xcb_request_check(g_xcbConnection, cookie)))
    {
        LOG_ERROR("Failed to create XCB window (error code %i)", error->error_code);
        free(error);
        return false;
    }

    SCOPE_GUARD([this, windowPrivate]()
    {
        if(!m_open)
        {
            const xcb_void_cookie_t cookie = xcb_destroy_window_checked(g_xcbConnection, windowPrivate->window);
            if(xcb_generic_error_t* error = xcb_request_check(g_xcbConnection, cookie))
            {
                LOG_WARNING("Failed to destroy XCB window (error code %i)", error->error_code);
                free(error);
            }
        }
    });

    void* userData = this;
    cookie = xcb_change_property_checked(
        g_xcbConnection,
        XCB_PROP_MODE_REPLACE,
        windowPrivate->window,
        g_xcbAtomUserData,
        XCB_ATOM_ATOM,
        32, 2,
        &userData);

    if((error = xcb_request_check(g_xcbConnection, cookie)))
    {
        LOG_ERROR("Failed to change user data for XCB window (error code %i)", error->error_code);
        free(error);
        return false;
    }

    cookie = xcb_change_property_checked(
        g_xcbConnection,
        XCB_PROP_MODE_REPLACE,
        windowPrivate->window,
        g_xcbAtomWMProtocols,
        XCB_ATOM_ATOM,
        32, 1,
        &g_xcbAtomWMDeleteWindow);

    if((error = xcb_request_check(g_xcbConnection, cookie)))
    {
        LOG_ERROR("Failed to override XCB window close (error code %i)", error->error_code);
        free(error);
        return false;
    }

    cookie = xcb_map_window_checked(g_xcbConnection, windowPrivate->window);
    if((error = xcb_request_check(g_xcbConnection, cookie)))
    {
        LOG_ERROR("Failed to map XCB window (error code %i)", error->error_code);
        free(error);
        return false;
    }

    if(const int result = xcb_flush(g_xcbConnection) <= 0)
    {
        LOG_WARNING("Failed to flush XCB connection (error code %i)", result);
    }

    xcb_get_geometry_cookie_t geometryCookie = xcb_get_geometry(g_xcbConnection, windowPrivate->window);
    if(xcb_get_geometry_reply_t* geometryReply = xcb_get_geometry_reply(g_xcbConnection, geometryCookie, &error))
    {
        m_width = geometryReply->width;
        m_height = geometryReply->height;
        free(geometryReply);
    }
    else
    {
        LOG_WARNING("Failed to retrieve XCB window size (error code %i)", error->error_code);
        free(error);
    }

    m_open = true;
    UpdateTitle();

    LOG_SUCCESS("Created XCB window");
    return true;
}

void Platform::Window::OnClose()
{
    ASSERT(m_private);
    const auto* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    ASSERT_SLOW(g_xcbConnection);

    const xcb_void_cookie_t cookie = xcb_destroy_window_checked(g_xcbConnection, windowPrivate->window);
    if(xcb_generic_error_t* error = xcb_request_check(g_xcbConnection, cookie))
    {
        LOG_WARNING("Failed to destroy XCB window (error code %i)", error->error_code);
        free(error);
    }

    CloseXCBConnection();
}

void Platform::Window::OnResize(const u32 width, const u32 height)
{
    ASSERT(m_private);
    const auto* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    ASSERT_SLOW(windowPrivate->window);
    ASSERT_SLOW(g_xcbConnection);

    const u32 values[] = { width, height };
    const xcb_void_cookie_t cookie = xcb_configure_window_checked(g_xcbConnection,
        windowPrivate->window, XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT, values);

    if(xcb_generic_error_t* error = xcb_request_check(g_xcbConnection, cookie))
    {
        LOG_ERROR("Failed to resize XCB window (error code %i)", error->error_code);
        free(error);
    }
}

void Platform::Window::OnUpdateTitle(const char* title)
{
    ASSERT(m_private);
    const auto* windowPrivate = static_cast<WindowPrivate*>(m_private.Get());
    ASSERT_SLOW(windowPrivate->window);
    ASSERT_SLOW(g_xcbConnection);

    const xcb_void_cookie_t cookie = xcb_change_property_checked(
        g_xcbConnection,
        XCB_PROP_MODE_REPLACE,
        windowPrivate->window,
        g_xcbAtomWMName,
        g_xcbAtomUTF8String,
        sizeof(char) * 8,
        strlen(title),
        title);

    if(xcb_generic_error_t* error = xcb_request_check(g_xcbConnection, cookie))
    {
        LOG_ERROR("Failed to update XCB window title (error code %i)", error->error_code);
        free(error);
    }
}

const char* Platform::Window::GetVulkanSurfaceExtension()
{
    return VK_KHR_XCB_SURFACE_EXTENSION_NAME;
}
