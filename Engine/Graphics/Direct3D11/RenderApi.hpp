#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    struct RenderConfig;
}

namespace Graphics::Detail
{
    class RenderApi final : NonCopyable
    {
        ComPtr<ID3D11Device5> m_device;
        ComPtr<ID3D11DeviceContext4> m_context;
        ComPtr<IDXGISwapChain4> m_swapchain;
        ComPtr<ID3D11RenderTargetView> m_swapchainView;
        HANDLE m_swapchainFrameWaitable = nullptr;

    public:
        RenderApi() = default;
        ~RenderApi();

        bool Setup(const Platform::Window* window, const RenderConfig& config);
        void Resize(u32 width, u32 height);

        void WaitForFrame() const;
        void BeginFrame(u32 width, u32 height);
        void EndFrame();

        ID3D11Device5* GetDevice() const
        {
            ASSERT(m_device);
            return m_device.Get();
        }

        ID3D11DeviceContext4* GetContext() const
        {
            ASSERT(m_context);
            return m_context.Get();
        }

    private:
        bool CreateDevice(const RenderConfig& config);
        bool CreateSwapchain(const Platform::Window* window);
        bool CreateSwapchainView();
        bool CreateSwapchainSync();
        void ResizeSwapchain(u32 width, u32 height);
    };
}
