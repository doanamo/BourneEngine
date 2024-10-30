#pragma once

#include "Graphics/Stats.hpp"

namespace Platform
{
    class Window;
    class TimeSlice;
}

// #todo: Header file shouldn't be "System.hpp" but "GraphicsSystem.hpp"
namespace Graphics
{
    class System final
    {
    public:
        System() = default;
        ~System();

        System(const System&) = delete;
        System& operator=(const System&) = delete;

        bool Setup(const Platform::Window* window);
        void BeginFrame();
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

        const Stats& GetStats() const
        {
            return m_stats;
        }

    private:
        bool CreateDevice();
        bool CreateSwapchain();
        bool CreateRenderTargetView();

        const Platform::Window* m_window = nullptr;

        ComPtr<ID3D11Device5> m_device;
        ComPtr<ID3D11DeviceContext1> m_deviceContext;
        ComPtr<ID3D11RenderTargetView> m_renderTargetView;
        ComPtr<IDXGISwapChain4> m_swapchain;

        Stats m_stats;
    };
}
