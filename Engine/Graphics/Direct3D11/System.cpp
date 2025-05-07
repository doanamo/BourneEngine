#include "Shared.hpp"
#include "System.hpp"
#include "Graphics/System.hpp"
#include "Platform/Window.hpp"

Graphics::Detail::System::~System()
{
    m_renderTargetView = nullptr;
    m_swapchain = nullptr;
    m_deviceContext = nullptr;
    m_device = nullptr;

#ifdef ENABLE_GRAPHICS_DEBUG
    ComPtr<IDXGIDebug1> dxgiDebug;
    if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiDebug))))
    {
        dxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL,
            static_cast<DXGI_DEBUG_RLO_FLAGS>(DXGI_DEBUG_RLO_SUMMARY | DXGI_DEBUG_RLO_IGNORE_INTERNAL));
    }
#endif
}

bool Graphics::System::OnSetup()
{
    if(!m_detail.CreateDevice())
        return false;

    if(!m_detail.CreateSwapchain(m_window))
        return false;

    if(!m_detail.CreateRenderTargetView())
        return false;

    return true;
}

bool Graphics::Detail::System::CreateDevice()
{
    UINT createDeviceFlags = 0;

#ifdef ENABLE_GRAPHICS_DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

    ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
    if(SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&dxgiInfoQueue))))
    {
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_WARNING, true)));
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true)));
        ASSERT_EVALUATE(SUCCEEDED(dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true)));
    }
#endif

    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    if(FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        nullptr,0, D3D11_SDK_VERSION, &device, nullptr, &context)))
    {
        LOG_ERROR("Failed to create D3D11 device");
        return false;
    }

    if(FAILED(device.As(&m_device)))
    {
        LOG_ERROR("Failed to query D3D11 device interface");
        return false;
    }

    if(FAILED(context.As(&m_deviceContext)))
    {
        LOG_ERROR("Failed to query D3D11 context interface");
        return false;
    }

    LOG("Created D3D11 device");
    return true;
}

bool Graphics::Detail::System::CreateSwapchain(const Platform::Window* window)
{
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

    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    swapchainDesc.Width = window->GetWidth();
    swapchainDesc.Height = window->GetHeight();
    swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchainDesc.SampleDesc.Count = 1;
    swapchainDesc.SampleDesc.Quality = 0;
    swapchainDesc.Scaling = DXGI_SCALING_NONE;
    swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchainDesc.BufferCount = 2;
    swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

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

    LOG("Created D3D11 swapchain");
    return true;
}

bool Graphics::Detail::System::CreateRenderTargetView()
{
    ComPtr<ID3D11Texture2D> backbuffer;
    if(FAILED(m_swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer))))
    {
        LOG_ERROR("Failed to get D3D11 backbuffer");
        return false;
    }

    if(FAILED(m_device->CreateRenderTargetView(backbuffer.Get(), nullptr, &m_renderTargetView)))
    {
        LOG_ERROR("Failed to create D3D11 render target view");
        return false;
    }

    LOG("Created D3D11 render target view");
    return true;
}

void Graphics::System::OnBeginFrame()
{
    ASSERT_SLOW(m_window);

    constexpr f32 ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_detail.m_deviceContext->ClearRenderTargetView(m_detail.m_renderTargetView.Get(), &ClearColor[0]);

    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<f32>(m_window->GetWidth());
    viewport.Height = static_cast<f32>(m_window->GetHeight());
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;

    m_detail.m_deviceContext->RSSetViewports(1, &viewport);
    m_detail.m_deviceContext->OMSetRenderTargets(1, m_detail.m_renderTargetView.GetAddressOf(), nullptr);
}

void Graphics::System::OnEndFrame()
{
    m_detail.m_swapchain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}
