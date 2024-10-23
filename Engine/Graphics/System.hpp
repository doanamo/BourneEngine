#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class System final
    {
    public:
        System() = default;
        ~System();

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        bool Setup(const Platform::Window& window);
        void BeginFrame(const Platform::Window& window);
        void EndFrame();

        ID3D11Device5* GetDevice() const
        {
            ASSERT(m_device);
            return m_device.Get();
        }

        ID3D11DeviceContext1* GetDeviceContext() const
        {
            ASSERT(m_deviceContext);
            return m_deviceContext.Get();
        }
    
    private:
        bool CreateDevice();
        bool CreateSwapchain(const Platform::Window& window);
        bool CreateRenderTargetView();

        ComPtr<ID3D11Device5> m_device;
        ComPtr<ID3D11DeviceContext1> m_deviceContext;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<IDXGISwapChain4> m_swapchain;
    };
}
