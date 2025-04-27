#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class System;
}

namespace Graphics::Detail
{
    class System
    {
        friend Graphics::System;

        ComPtr<ID3D11Device5> m_device;
        ComPtr<ID3D11DeviceContext4> m_deviceContext;
        ComPtr<IDXGISwapChain4> m_swapchain;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    public:
        System() = default;
        ~System();

        bool CreateDevice();
        bool CreateSwapchain(Platform::Window* window);
        bool CreateRenderTargetView();

        ID3D11Device5* GetDevice() const
        {
            ASSERT(m_device);
            return m_device.Get();
        }

        ID3D11DeviceContext4* GetDeviceContext() const
        {
            ASSERT(m_deviceContext);
            return m_deviceContext.Get();
        }
    };
}
