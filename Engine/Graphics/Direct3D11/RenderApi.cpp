#include "Shared.hpp"
#include "RenderApi.hpp"
#include "Config.hpp"
#include "Platform/Window.hpp"

Graphics::Detail::RenderApi::~RenderApi()
{
    if(m_context)
    {
        m_context->ClearState();

        m_swapchainView = nullptr;
        m_swapchain = nullptr;
        m_context = nullptr;
        m_device = nullptr;

#if ENABLE_GRAPHICS_DEBUG
        ComPtr<IDXGIDebug1> dxgiDebug;
        if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
        {
            dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL,
                static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
        }
#endif
    }
}

bool Graphics::Detail::RenderApi::Setup(const Platform::Window* window, const RenderConfig& config)
{
    if(!CreateDevice(config))
        return false;

    if(!CreateSwapchain(window))
        return false;

    if(!CreateSwapchainView())
        return false;

    return true;
}

bool Graphics::Detail::RenderApi::CreateDevice(const RenderConfig& config)
{
    UINT createDeviceFlags = 0;

#if ENABLE_GRAPHICS_DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

    ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
    if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue))))
    {
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true)));
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true)));
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true)));
    }
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    D3D_DRIVER_TYPE driverType = config.software ? D3D_DRIVER_TYPE_WARP : D3D_DRIVER_TYPE_HARDWARE;;

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    if(FAILED(D3D11CreateDevice(nullptr, driverType, nullptr, createDeviceFlags,
        featureLevels, ArraySize(featureLevels), D3D11_SDK_VERSION,
        &device, nullptr, &context)))
    {
        LOG_ERROR("Failed to create D3D11 device");
        return false;
    }

    if(FAILED(device.As(&m_device)))
    {
        LOG_ERROR("Failed to query D3D11 device interface");
        return false;
    }

    if(FAILED(context.As(&m_context)))
    {
        LOG_ERROR("Failed to query D3D11 context interface");
        return false;
    }

    LOG_SUCCESS("Created D3D11 device");
    return true;
}

bool Graphics::Detail::RenderApi::CreateSwapchain(const Platform::Window* window)
{
    ASSERT(m_device);
    ASSERT(!m_swapchain);

    ASSERT(window);
    HWND windowHandle = window->GetDetail().GetHandle();

    ComPtr<IDXGIDevice4> dxgiDevice;
    if(FAILED(m_device.As(&dxgiDevice)))
    {
        LOG_ERROR("Failed to query DXGI device interface");
        return false;
    }

    ComPtr<IDXGIAdapter> adapter;
    if(FAILED(dxgiDevice->GetAdapter(&adapter)))
    {
        LOG_ERROR("Failed to query DXGI adapter interface");
        return false;
    }

    ComPtr<IDXGIFactory7> factory;
    if(FAILED(adapter->GetParent(IID_PPV_ARGS(&factory))))
    {
        LOG_ERROR("Failed to get DXGI factory");
        return false;
    }

    // #todo: Support multisampling. This will require a separate MSAA buffer that is then resolved to backbuffer.
    // See: https://stackoverflow.com/questions/56286975/how-to-fix-this-multisampling-error-when-creating-a-swapchain

    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    swapchainDesc.Width = window->GetWidth();
    swapchainDesc.Height = window->GetHeight();
    swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapchainDesc.Flags = 0;

    ComPtr<IDXGISwapChain1> swapchain;
    if(FAILED(factory->CreateSwapChainForHwnd(m_device.Get(), windowHandle,
        &swapchainDesc, nullptr, nullptr, &swapchain)))
    {
        LOG_ERROR("Failed to create D3D11 swapchain");
        return false;
    }

    if(FAILED(swapchain.As(&m_swapchain)))
    {
        LOG_ERROR("Failed to query D3D11 swapchain interface");
        return false;
    }

    if(FAILED(factory->MakeWindowAssociation(windowHandle, DXGI_MWA_NO_ALT_ENTER)))
    {
        LOG_ERROR("Failed to make window association via DXGI factory");
        return false;
    }

    LOG_SUCCESS("Created D3D11 swapchain");
    return true;
}

bool Graphics::Detail::RenderApi::CreateSwapchainView()
{
    ASSERT(m_device);
    ASSERT(!m_swapchainView);

    ComPtr<ID3D11Texture2D> backbuffer;
    if(FAILED(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer))))
    {
        LOG_ERROR("Failed to get D3D11 backbuffer");
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC swapchainViewDesc = {};
    swapchainViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    swapchainViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    if(FAILED(m_device->CreateRenderTargetView(backbuffer.Get(), &swapchainViewDesc, &m_swapchainView)))
    {
        LOG_ERROR("Failed to create D3D11 swapchain view");
        return false;
    }

    LOG_SUCCESS("Created D3D11 swapchain view");
    return true;
}

void Graphics::Detail::RenderApi::ResizeSwapchain(u32 width, u32 height)
{
    ASSERT(m_context);
    ASSERT(m_swapchain);

    m_context->OMSetRenderTargets(0, nullptr, nullptr);
    m_swapchainView.Reset();

    if(FAILED(m_swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0)))
    {
        LOG_ERROR("Failed to resize D3D11 swapchain");
        return;
    }

    CreateSwapchainView();
}

void Graphics::Detail::RenderApi::Resize(u32 width, u32 height)
{
    ResizeSwapchain(width, height);
}

void Graphics::Detail::RenderApi::BeginFrame(u32 width, u32 height)
{
    ASSERT(m_context);
    ASSERT(m_swapchain);

    m_context->ClearState();

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<f32>(width);
    viewport.Height = static_cast<f32>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_context->RSSetViewports(1, &viewport);
    m_context->OMSetRenderTargets(1, m_swapchainView.GetAddressOf(), nullptr);

    constexpr f32 ClearColor[4] = { 0.0f, 0.25f, 0.25f, 1.0f };
    m_context->ClearRenderTargetView(m_swapchainView.Get(), &ClearColor[0]);
}

void Graphics::Detail::RenderApi::EndFrame()
{
    ASSERT(m_swapchain);
    if(FAILED(m_swapchain->Present(0, 0)))
    {
        LOG_ERROR("Failed to present D3D11 swapchain");
    }
}
