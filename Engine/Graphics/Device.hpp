#pragma once

namespace Platform
{
    class Window;
}

namespace Graphics
{
    class Device final
    {
    public:
        Device() = default;
        ~Device();

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;

        bool Setup(const Platform::Window& window);
        void BeginFrame();
        void EndFrame();

    private:
#ifdef GRAPHICS_DIRECT3D12
        static constexpr u32 SwapChainFrameCount = 3;

        bool CreateDevice();
        bool CreateCommandQueue();
        bool CreateSwapChain(const Platform::Window& window);
        bool CreateCommandList();
        bool CreateFrameSynchronization();

        void WaitForGPU();
        void AdvanceFrame();

        ComPtr<IDXGIFactory7> m_dxgiFactory;
        ComPtr<ID3D12Device10> m_device;
        ComPtr<ID3D12CommandQueue> m_commandQueue;
        ComPtr<ID3D12CommandAllocator> m_commandAllocator[SwapChainFrameCount];
        ComPtr<ID3D12GraphicsCommandList> m_commandList;
        ComPtr<IDXGISwapChain4> m_swapChain;
        ComPtr<ID3D12DescriptorHeap> m_swapChainViewHeap;
        ComPtr<ID3D12Resource> m_swapChainViews[SwapChainFrameCount];

        u32 m_frameIndex = 0;
        u64 m_frameFenceValues[SwapChainFrameCount] = {};
        ComPtr<ID3D12Fence> m_frameFence;
        HANDLE m_frameFenceEvent;
#endif
    };
}
