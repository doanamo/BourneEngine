#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    struct SystemConfig;
}

namespace Graphics::Detail
{
    class System final : NonCopyable
    {
        ComPtr<ID3D11Device5> m_device;
        ComPtr<ID3D11DeviceContext4> m_context;
        ComPtr<IDXGISwapChain4> m_swapchain;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;

    public:
        System() = default;
        ~System();

        bool Setup(const Platform::Window* window, const SystemConfig& config);
        void BeginFrame(const Platform::Window* window);
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
        bool CreateDevice(const SystemConfig& config);
        bool CreateSwapchain(const Platform::Window* window);
        bool CreateRenderTargetView();
    };
}
