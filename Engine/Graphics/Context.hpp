#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    // #todo: Rename to device
    class Context final
    {
    public:
        Context() = default;
        ~Context();

        Context(const Context&) = delete;
        Context& operator=(const Context&) = delete;

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
